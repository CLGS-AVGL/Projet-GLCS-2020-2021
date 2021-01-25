#pragma once

// library headers
#include <configuration.hpp>

class CommandLineConfig:
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

  /// name of the input dataset containing initial conditions
  std::string m_input_dataset;
  
  /// name of the output file used for backups
  std::string m_output_filename;

  /// interval between full backups
  int m_backup_interval;

  /// interval between mean backups
  int m_mean_backup_interval; 

public:
	/** Construct a new CommandLineConfig
	 * @param argc the number of command-line arguments
	 * @param argv the values of command-line arguments
	 */
	CommandLineConfig(const int argc, const char* const argv[]);
	
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
  std::string input_dataset() const override {return m_input_dataset; }
  
  // see overridden function
  std::string output_filename() const override { return m_output_filename; }
  
  // see overridden function
  int backup_interval() const override { return m_backup_interval; }

  // see overridden function
  int mean_backup_interval() const override { return m_mean_backup_interval; }
};
