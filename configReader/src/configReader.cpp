#include "configReader.hpp"
#include <string>

#define exist(filename, buffer) (stat(filename, buffer) == 0)

void alert(const std::string &to_print, int &error_code, const int rank)
{
  error_code = 1;
  if (rank == 0)
    std::cerr << std::endl << "Fatal-error: " << to_print << std::endl << std::endl;
}

void ConfigReader::init_options(po::options_description &options) const
{
  // add the opttions description
  options.add_options()
  ("N-iter",          po::value<int>()->default_value(10), 
   "number of iterations (default 10)")
  ("height",          po::value<int>()->default_value(4),  
   "height of the domain (default 4)")
  ("width",           po::value<int>()->default_value(8),
   "width of the domain (default 8)")
  ("process-height", po::value<int>()->default_value(2), 
   "number of process in the height (default 2)")
  ("process-width",  po::value<int>()->default_value(2),  
   "number of process in the width (default 2)")
  ("delta-t",        po::value<double>()->default_value(0.125), 
   "time elapsed between two iterations (default 0.125)")
  ("delta-x",        po::value<double>()->default_value(1),  
   "distance on the x axis between two points of the domain (default 1)")
  ("delta-y",        po::value<double>()->default_value(1), 
   "distance on the y axis between two points of the domain (default 1)")
  ("config-file",        po::value<std::string>(),
   "config-file name used to initialize the configuration (no default value)")
  ("input-file",        po::value<std::string>(), 
   "input-file name used for initial conditions (no default value)")
  ("output-file",       po::value<std::string>(),
   "output-file used to save results (no default value)")
  ("backup-interval",   po::value<int>()->default_value(1),   
   "interval between backups (default 1)")
  ("help", "produce help message")
  ("print-args,pa", "print the command-line arguments");
}

void ConfigReader::print_help(const po::variables_map &vm, 
    const po::options_description &options, const int rank) const
{
  if(vm.count("help")){
    // if the rank is 0 print the options
    if (rank == 0){
      std::cout << options << std::endl;
    }

    // close the program
    MPI_Finalize();
    exit(0);
  }
}

void ConfigReader::parse_arguments(po::variables_map &vm, const po::options_description &options, 
    const int argc, const char* const argv[]) const
{
  po::positional_options_description p;
  p.add("config-file", -1);

  po::store(po::command_line_parser(argc, argv).options(options).positional(p).run(), vm);
  po::notify(vm);
}

void ConfigReader::set_commmand_line(const po::variables_map &vm, const int rank)
{
  // set the parameters
  m_nb_iter = vm["N-iter"].as<int>();
	m_global_shape[DY] = vm["height"].as<int>();
	m_global_shape[DX] = vm["width"].as<int>();
	m_dist_extents[DY] = vm["process-height"].as<int>();
	m_dist_extents[DX] = vm["process-width"].as<int>();
	m_delta_t = vm["delta-t"].as<double>();
	m_delta_space[DY] =vm["delta-y"].as<double>();
	m_delta_space[DX] =vm["delta-x"].as<double>();
  if (vm.count("input-file")) 
    m_input_filename = vm["input-file"].as<std::string>();
  if (vm.count("output-file")) 
    m_output_filename = vm["output-file"].as<std::string>();
  m_backup_interval =vm["backup-interval"].as<int>();
}

void ConfigReader::set_config_file(const std::string &filename, const int rank)
{
  // inform the user
  if (rank == 0){
    std::cout << std::endl;
    std::cout << "Information: When a config file is given, only the arguments of this";
    std::cout << "file are used, except for \"print-args\" and \"help\"" << std::endl;
    std::cout << std::endl;
  }

  // create the property tree
  pt::ptree tree;

  // parse the config file
  pt::read_xml(filename, tree);

  // set the parameters
  m_nb_iter = tree.get("N-iter", 10);
	m_global_shape[DY] = tree.get("height", 4);
	m_global_shape[DX] = tree.get("width", 4);
	m_dist_extents[DY] = tree.get("process-height", 2);
	m_dist_extents[DX] = tree.get("process-width", 2);
	m_delta_t = tree.get("delta-t", 0.125); 
	m_delta_space[DY] = tree.get("delta-y", 1); 
	m_delta_space[DX] = tree.get("delta-x", 1); 
  
  boost::optional<std::string> in_opt = tree.get_optional<std::string>("input-file");
  if (in_opt) 
    m_input_filename = (*in_opt);
  
  boost::optional<std::string> out_opt = tree.get_optional<std::string>("output-file");
  if (out_opt) 
    m_output_filename = (*out_opt);
  
  m_backup_interval = tree.get("backup-interval", 1);
}

void ConfigReader::print_config(const po::variables_map &vm, const int rank) const
{
  if (vm.count("print-args") && rank == 0){
    std::cout << "************** Configutation **************" << std::endl;
    std::cout << "m_nb_iter = "          <<  m_nb_iter         << std::endl; 
	  std::cout << "m_global_shape[DY] = " << m_global_shape[DY] << std::endl; 
	  std::cout << "m_global_shape[DX] = " << m_global_shape[DX] << std::endl;
	  std::cout << "m_dist_extents[DY] = " << m_dist_extents[DY] << std::endl;
	  std::cout << "m_dist_extents[DX] = " << m_dist_extents[DX] << std::endl;
	  std::cout << "m_delta_t = "          << m_delta_t          << std::endl;
	  std::cout << "m_delta_space[DY] = "  << m_delta_space[DY]  << std::endl;
	  std::cout << "m_delta_space[DX] = "  << m_delta_space[DX]  << std::endl;
    if (!m_input_filename.empty())   std::cout << "m_input_filename = "   << m_input_filename   << std::endl;
    if (!m_output_filename.empty()) std::cout << "m_output_filename = "  << m_output_filename  << std::endl;
    std::cout << "m_backup_interval = "  << m_backup_interval  << std::endl;
    std::cout << "*******************************************" << std::endl;
    std::cout << std::endl; 
  }
}

void ConfigReader::check_parameters(const int rank, const int size) const
{
  int error_code = 0;

  if (m_nb_iter < 0)
	  alert("The number of iterations must be positive or zero, is " + std::to_string(m_nb_iter), error_code, rank); 

  if (m_global_shape[DY] < 0)
    alert("The field height must be positive or zero, is " + std::to_string(m_global_shape[DY]), error_code, rank); 
	
  if (m_global_shape[DX] < 0)
    alert("The field width must be positive or zero, is " + std::to_string(m_global_shape[DX]), error_code, rank);

  if (m_dist_extents[DY] < 0) 
    alert("The number of processus on the y axis must be positive, is " + std::to_string(m_dist_extents[DY]), error_code, rank);

  if (m_dist_extents[DX] < 0)
    alert("The number of processus on the x axis must be positive, is " + std::to_string(m_dist_extents[DY]), error_code, rank);
  
  if (m_dist_extents[DX] * m_dist_extents[DY] > size)
    alert("Not enough processus, " + std::to_string(m_dist_extents[DX]) + "*" + 
        std::to_string(m_dist_extents[DY]) + "!=" + std::to_string(size) , error_code, rank);

  if (m_delta_t <= 0)
    alert("The delta t must be positive, is " + std::to_string(m_delta_t), error_code, rank);

  if (m_delta_space[DY] <= 0)
    alert("The distance between points on the y axis must be positive, is " + std::to_string(m_delta_space[DY]), error_code, rank);
  
  if (m_delta_space[DX] <= 0)
    alert("The distance between points on the x axis must be positive, is " + std::to_string(m_delta_space[DX]), error_code, rank);

  if (m_backup_interval < 1)
    alert("The backup interval must be superior or equal to 1, is " + std::to_string(m_backup_interval), error_code, rank); 

  struct stat buffer;
  
  if (!m_input_filename.empty() && !exist(m_input_filename.c_str(), &buffer))
    alert("The given input file cannot be found", error_code, rank);

  if (!m_output_filename.empty() && !exist(m_output_filename.c_str(), &buffer))
    alert("The given output_filename file cannot be found", error_code, rank);

  if (error_code != 0){
    MPI_Finalize();
    exit(error_code);
  }

}

ConfigReader::ConfigReader( const int argc, const char* const argv[])
{
  // get the processus rank
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // get a short name for the boost name_space
  
  // create the options description object
  po::options_description options("Options");
 
  // initialize options 
  init_options(options);

  // parse the command line and extract the variables map
  po::variables_map vm;
  parse_arguments(vm, options, argc, argv);
  
  // help message
  print_help(vm, options, rank);

  if (vm.count("config-file")){
    // set the parameters form the config file
    set_config_file(vm["config-file"].as<std::string>(), rank);
  }else{
    // set the parameters from the command line
    set_commmand_line(vm, rank);
  }

  // print the configuration
  print_config(vm, rank);

  // check parameters
  check_parameters(rank, size);
}
