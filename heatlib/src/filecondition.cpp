// the implemented class (last)
#include "fileconditions.hpp"

void FileConditions::initial_condition( Distributed2DField& data ,
					const Configuration& config) const
{
	// initialize everything to 0
	for ( int yy = 0; yy < data.full_view().extent( DY ); ++yy ) {
		for ( int xx = 0; xx < data.full_view().extent( DX ); ++xx ) {
			data.full_view( yy, xx ) = 0;;
		}
	}

	// except the boundary condition on the left if our block is at the boundary itself
	if ( data.distribution().coord( DX ) == 0 ) {
		Distributed2DField::View left_ghost = data.ghost_view( LEFT );
		for ( int yy = 0; yy < left_ghost.extent( DY ); ++yy ) {
			for ( int xx = 0; xx < data.full_view().extent( DX ); ++xx ) {
				left_ghost( yy, 0 ) = 2097.152;
			}
		}
	}

	
	std::string dataset_name = config.input_dataset();

	// test MPI_init call
	if( !MPI::Is_initialized() )
	  {
	    std::cerr << "Call MPI_COMM_SIZE before MPI_Init() call, abort()" << std::endl;
	    abort();
	  }


	// get Communicator infos
	int rank;
	int size;
	MPI_Comm Communicator = data.distribution().communicator();
	rank = data.distribution().rank();
	size = data.distribution().size();
  
	// property lists for MPI-HDF5 use
	hid_t plist_id=  H5Pcreate(H5P_FILE_ACCESS);
	H5Pset_fapl_mpio(plist_id,
			 Communicator,
			 MPI_INFO_NULL);

	hid_t plist_read_id = H5Pcreate(H5P_DATASET_XFER);
	H5Pset_dxpl_mpio(plist_read_id, H5FD_MPIO_COLLECTIVE);

	// disable error message
	H5Eset_auto(NULL,NULL,NULL);

	hid_t h5file;
	
	// test if the file exist
	h5file = H5Fopen(config.input_filename().c_str(), H5F_ACC_RDONLY, plist_id);
	if(h5file < 0)
	  {
	    std::cerr << "the file : [" <<config.input_filename() << "] doesnt exist. abort."  << std::endl;
	    abort();
	  }

	// open the group "/"
	hid_t group_id = H5Gopen(h5file, "/", H5P_DEFAULT);
	if(group_id < 0)
	  {
	    std::cerr << "the group '/' ] doesnt exist. abort."  << std::endl;
	    abort();
	  }

	// test if the dataset exist
	hid_t initial_dataset = H5Dopen(group_id, dataset_name.c_str(), H5P_DEFAULT);
	if(initial_dataset < 0)
	  {
	    std::cerr << "the dataset : [" <<config.input_dataset() << "] doesnt exist. abort."  << std::endl;
	    abort();
	  }

	// reactivate error message
	H5Eset_auto(NULL,NULL,NULL);
	
	
	// verify that the data dimensions are compatible with what's needed now
	// s'inspirer de hdf5writer
	
	// read data (select hyperslab in file)
	hsize_t start_w[2]  = {0, 0};
	const hsize_t stride_w[2] = {1,1};
	const hsize_t count_w[2]  = {1,1};
	const hsize_t block_size[2] = {data.noghost_view().extent(DY), data.noghost_view().extent(DX)};
	start_w[0] = data.distribution().coord(DY) * data.noghost_view().extent(DY); // how to find the begin-height
	start_w[1] = data.distribution().coord(DX) * data.noghost_view().extent(DX); // how to find the begin-width
	hid_t fspace_id = H5Dget_space(initial_dataset);
	herr_t err_code_slab = H5Sselect_hyperslab (fspace_id,
						    H5S_SELECT_SET,
						    start_w,
						    stride_w,
						    count_w,
						    block_size);
	double ptr[data.noghost_view().extent(DY) *
		   data.noghost_view().extent(DX)* sizeof(double)];

	hid_t memspace_id = H5Screate_simple( data.NDIM, block_size, NULL);
	
	herr_t err_read = H5Dread(initial_dataset,
				  H5T_NATIVE_DOUBLE,
				  memspace_id,
				  fspace_id,
				  plist_read_id,
				  ptr);
	
	// fill the no_ghost_view (line inversion ? see the write trick)
	for(int i = data.noghost_view().extent(DY) ; i >= 0 ; i--)
	  for(int j = 0 ; j < data.noghost_view().extent(DX) ; j++)
	    data.noghost_view(i,j) = ptr[i * data.noghost_view().extent(DX) + j];

	// close all hdf5 objects
	H5Dclose(initial_dataset);
	H5Sclose(fspace_id);
	H5Gclose(group_id);
	H5Pclose(plist_id);
	H5Pclose(plist_read_id);
	H5Fclose(h5file);
}
