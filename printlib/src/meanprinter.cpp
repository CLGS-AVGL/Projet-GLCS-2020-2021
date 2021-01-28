// headers
#include "meanprinter.hpp"
// utiliser un gather pour avoir la moyenne des moyennes
// seul master va ecrire dans un fichier hdf5, pas de property list mpi ici !
void write_mean(const Distributed2DField &data, const Configuration &config) {
  
  // test MPI_init call
  int init_flag;
  MPI_Initialized(&init_flag);

  if (!init_flag) {
    std::cerr << "Call MPI_COMM_SIZE before MPI_Init() call, abort()"
              << std::endl;
    abort();
  }

  // get Communicator infos
  double mean_heat = 0;
  double local_mean = data.reducedValue();
  int rank;
  int size;
  MPI_Comm Communicator = data.distribution().communicator();
  rank = data.distribution().rank();
  size = data.distribution().size();

  MPI_Reduce(&local_mean, &mean_heat, 1, MPI_DOUBLE, MPI_SUM, 0, Communicator);

  if(rank == 0) {
  // compute the mean of means
  mean_heat /= size;
  
  // create a file with an explicit name each time it's necessary
  std::string file_name = std::string("mean_") + config.output_filename();
  hid_t h5file;

  // disable error message
  H5Eset_auto(NULL, NULL, NULL);

  // try to open a file, success if it wasnt existant, fail otherwise
  h5file = H5Fcreate(file_name.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);

  // if failed to create, try to open
  if (h5file < 0) {
    h5file = H5Fopen(file_name.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
    if (h5file < 0) {
      std::cerr << "fail to open file. exit(-1)" << std::endl;
      exit(-1);
    }
  }

  // reactivate error message
  H5Eset_auto(NULL, NULL, NULL);

  // the total matrix size to write
  const hsize_t block[2] = {1, 1};

  const hsize_t block_size[2] = {1, 1};

  hsize_t start_w[2] = {0, 0};
  const hsize_t stride_w[2] = {1, 1};
  const hsize_t count_w[2] = {1, 1};

  hid_t dataset_id;
  hid_t dataspace_id;

  //                           NDIM = 2
  dataspace_id = H5Screate_simple(data.NDIM, block, NULL);

  double time = data.time();
  std::string str_time = std::to_string(time);

  // use m_time to create dataset depending on time t
  dataset_id = H5Dcreate(h5file, str_time.c_str(), H5T_NATIVE_DOUBLE,
                         dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  if (dataset_id < 0) {
    std::cerr << "error in H5Dcreate, in meanprinter.cpp, exit." << std::endl;
    exit(-1);
  }

  hid_t fspace_id = H5Dget_space(dataset_id);

  // select hyperslab in file
  start_w[0] = 0;
  start_w[1] = 0;
  herr_t err_code_slab = H5Sselect_hyperslab(fspace_id, H5S_SELECT_SET, start_w,
                                             stride_w, count_w, block_size);
  // select hyperslab in memory
  const hsize_t start[2] = {0, 0};
  const hsize_t stride[2] = {1, 1};
  const hsize_t count[2] = {1, 1};

  hid_t mem_space = H5Screate_simple(data.NDIM, block_size, NULL);

  herr_t err_code = H5Sselect_hyperslab(mem_space, H5S_SELECT_SET, start,
                                        stride, count, block_size);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, mem_space, fspace_id, H5P_DEFAULT,
           &mean_heat);

  // release
  H5Dclose(dataset_id);
  H5Sclose(dataspace_id);
  H5Fclose(h5file);
  }
}

void MeanPrinter::simulation_updated(const Distributed2DField &data,
                                     const Configuration &config) {
  bool its_time_to_write_mean = false;
  double nb_dt = data.time() / config.delta_t();
  int nb = static_cast<int>(nb_dt);
  if (nb % config.mean_backup_interval() == 0) {
    its_time_to_write_mean = true;
  }

  if (its_time_to_write_mean) {
    write_mean(data, config);
  }
}
