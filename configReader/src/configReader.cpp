#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#include "configReader.hpp"

using std::cerr;
using std::endl;
using std::exit;
using std::stoi;
using std::stod;
using std::cout;

ConfigReader::ConfigReader( const int argc, const char* const argv[] )
{
  // get a short name for the boost name_space
  namespace po = boost::program_options;
  
  // create the options description object
  po::options_description options("Options");
  
  // add the opttions description
  options.add_options()
  ("N-iter,n",          po::value<int>(),         "number of iterations")
  ("height,h",          po::value<int>(),         "height of the domain")
  ("width,w",           po::value<int>(),         "width of the domain")
  ("process-height,ph", po::value<int>(),         "number of process in the height")
  ("process-width,pw",  po::value<int>(),         "number of process in the width")
  ("delta-t,dt",        po::value<double>(),      "time elapsed between two iterations")
  ("delta-x,dx",        po::value<double>(),      "distance on the x axis between two points of the domain")
  ("delta-y,dy",        po::value<double>(),      "distance on the y axis between two points of the domain")
  ("input-file",        po::value<std::string>(), "input-file name used for initial conditions")
  ("output-file",       po::value<std::string>(), "output-file used to save results")
  ("backup-interval",   po::value<int>(),         "interval between backups")
  ("print-args,pa", "print the command-line arguments");

  // create the map that store the value of the options
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, options), vm);
  po::notify(vm);

  // set the parameters
  m_nb_iter = (vm.count("N-iter")) ? vm["N-iter"].as<int>() : 10;
	m_global_shape[DY] = (vm.count("height")) ? vm["height"].as<int>() : 4;
	m_global_shape[DX] = (vm.count("width")) ? vm["width"].as<int>() : 8;
	m_dist_extents[DY] = (vm.count("process-height")) ? vm["process-height"].as<int>() : 2;
	m_dist_extents[DX] = (vm.count("process-width")) ? vm["process-width"].as<int>() : 2;
	m_delta_t = (vm.count("delta-t")) ? vm["delta-t"].as<double>() : 0.125;
	m_delta_space[DY] = (vm.count("delta-y")) ? vm["delta-y"].as<double>() : 1;
	m_delta_space[DX] = (vm.count("delta-x")) ? vm["delta-x"].as<double>() : 1;
  if (vm.count("input-file")) m_input_filename = vm["input-file"].as<std::string>();
  if (vm.count("output-file")) m_output_filename = vm["output-file"].as<std::string>();
  m_backup_interval = (vm.count("backup-interval")) ? vm["backup-interval"].as<int>() : 1;

  if (vm.count("print-args")){
    std::cout << "************** Configutation **************" << std::endl;
    std::cout << "m_nb_iter = "          <<  m_nb_iter         << std::endl; 
	  std::cout << "m_global_shape[DY] = " << m_global_shape[DY] << std::endl; 
	  std::cout << "m_global_shape[DX] = " << m_global_shape[DX] << std::endl;
	  std::cout << "m_dist_extents[DY] = " << m_dist_extents[DY] << std::endl;
	  std::cout << "m_dist_extents[DX] = " << m_dist_extents[DX] << std::endl;
	  std::cout << "m_delta_t = "          << m_delta_t          << std::endl;
	  std::cout << "m_delta_space[DY] = "  << m_delta_space[DY]  << std::endl;
	  std::cout << "m_delta_space[DX] = "  << m_delta_space[DX]  << std::endl;
    std::cout << "m_input_filename = "   << m_input_filename   << std::endl;
    std::cout << "m_output_filename = "  << m_output_filename  << std::endl;
    std::cout << "m_backup_interval = "  << m_backup_interval  << std::endl;
    std::cout << std::endl; 
  }
}
