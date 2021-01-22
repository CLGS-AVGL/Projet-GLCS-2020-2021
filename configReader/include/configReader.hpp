#pragma once

#include <cstdlib>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <boost/optional/optional.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/range/end.hpp>

// library headers
#include <configuration.hpp>

using std::cerr;
using std::endl;
using std::exit;
using std::stoi;
using std::stod;
using std::cout;

namespace po = boost::program_options;
namespace pt = boost::property_tree;

class ConfigReader: 
   public Configuration
{ 
	/// number of iterations to execute
	int m_nb_iter;
	
	/// shape of the global data field
	Coord2D m_global_shape;
	
	/// shape of the data distribution
	Coord2D m_dist_extents;
	
	/// time difference between two consecutive points
	double m_delta_t;
	
	/// space difference between two consecutive points
	std::array<double, 2> m_delta_space;

  /// name of the input file used for inital conditions
  std::string m_input_filename;

  /// name of the output file used for backups
  std::string m_output_filename;

  /// interval between full backups
  int m_backup_interval;

  /// interval between mean backups
  int m_mean_backup_interval; 
 
  /** init the boost options
   *  @param options description of the options 
   */
  void init_options(po::options_description &options) const;

  /** print the help message if necessary
   *  @param vm the map of variables 
   *  @param options the options description
   *  @param rank the rank of the processus
   */
  void print_help(const po::variables_map &vm, const po::options_description &options, const int rank) const;
 
  /** print the help message if necessary
   *  @param vm the map of variables to fill 
   *  @param options the options description
   *  @param argc the number of arguments given to the program
   *  @param argv the arguments given to the program
   */
  void parse_arguments(po::variables_map &vm, const po::options_description &options, const int argc,
      const char* const argv[]) const;

  /** set the parameters from the command line
   *  @param vm the map of variables 
   *  @param rank the rank of the processus
   */
  void set_commmand_line(const po::variables_map &vm, const int rank);
  
  /** set the parameters from the config file
   *  @param std::string name of the config file
   *  @param rank the rank of the processus
   */
  void set_config_file(const std::string &filename, const int rank);

  /** print the config file if necessary
   *  @param vm the map of variables 
   *  @param rank the rank of the processus
   */
  void print_config(const po::variables_map &vm, const int rank) const;
 
  /** check if the parameters are correct
   *  @param vm the map of variables 
   *  @param rank the rank of the processus
   *  @param size of the communicator
   */
  void check_config(const int rank, const int size) const;

public:
	/** Construct a new ConfigReader and intialize the configuration from the command line or 
   * from the configuration file
	 * @param argc the number of command-line arguments
	 * @param argv the values of command-line arguments
	 */
	ConfigReader(const int argc, const char* const argv[]);
	
	// see overridden function
	int nb_iter() const override { return m_nb_iter; }
	
	// see overridden function
	Coord2D global_shape() const override { return m_global_shape; }
	
	// see overridden function
	Coord2D dist_extents() const override { return m_dist_extents; }
	
	// see overridden function
	double delta_t() const override { return m_delta_t; }
	
	// see overridden function
	std::array<double, 2> delta_space() const override { return m_delta_space; }

  // see overridden function
  std::string input_filename() const override { return m_input_filename; }

  // see overridden function
  std::string output_filename() const override { return m_output_filename; }
  
  // see overridden function
  int backup_interval() const override { return m_backup_interval; }

  // see overridden function
  int mean_backup_interval() const override { return m_mean_backup_interval; }
};
