// #include <hdf5/serial/hdf5.h>
// #include <stdio.h>
// #include <stdlib.h>
// #define FILE            "/home/nick/projects/fluids_viewer/build/particle_files/particles_pos_100000.h5"//"h5ex_t_array.h5"
// #define DATASET         "DS1"
// #define DIM0            4
// #define ADIM0           3
// 
// int main (void)
// {
//     hid_t       file, filetype, memtype, space, dset;                       /* Handles */
//     herr_t      status;
//     hsize_t     dims[1] = {DIM0},   adims[1] = {ADIM0};
//     float         **rdata;                                                    /* Read buffer */
//     int            ndims, i, j;
//                                                                             /* Now we begin the read section of this example.  Here we assume
//                                                                             * the dataset and array have the same name and rank, but can have
//                                                                             * any size.  Therefore we must allocate a new array to read in
//                                                                             * data using malloc().*/
//     file = H5Fopen (FILE, H5F_ACC_RDONLY, H5P_DEFAULT);                     /* Open file and dataset. */
//     printf("\nfile=%i\t",(int)file);
//     dset = H5Dopen (file, DATASET, H5P_DEFAULT);
//     filetype = H5Dget_type (dset);                                          /* Get the datatype and its dimensions.*/
//     ndims = H5Tget_array_dims (filetype, adims);
//     printf("\nfiletype=%i, ndims=%i \n",(int)filetype,ndims);
//                                                                             /* Get dataspace and allocate memory for read buffer.  This is a
//                                                                             * three dimensional dataset when the array datatype is included so
//                                                                             * the dynamic allocation must be done in steps. */
//     space = H5Dget_space (dset);
//     ndims = H5Sget_simple_extent_dims (space, dims, NULL);      
//     rdata       = (float **) malloc (dims[0] * sizeof (float *));                       /* 2D array of pointers to rows in the data elements.*/
//     rdata[0]    = (float *) malloc (dims[0] * adims[0] * sizeof (float));               /* Allocate space for integer data.*/
// 
//     /* Set the members of the pointer arrays allocated above to point to the correct locations in their respective arrays.*/
//     for (i=0; i<dims[0]; i++) {
//         rdata[i] = rdata[0] + i * adims[0];
//         for (j=0; j<adims[0]; j++)
//             rdata[i][j] = rdata[0][0] + (adims[0] * i) + j;
//     }
//     memtype = H5Tarray_create (H5T_NATIVE_INT, 2, adims);                           /* Create the memory datatype.*/
//     status = H5Dread (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT,  rdata[0]);     /* Read the data.*/    
//     
//     printf("dims[0]=%i, adims[0]=%i\n",(int)dims[0],(int)adims[0]);
//     for (i=0; i<1; i++) {                                                           /* Output one line of data to the screen.*/
//         printf ("%s[%d]:\n", DATASET, i);
//         for (j=0; j<adims[0]; j++)  printf (" %3f", rdata[i][j]);
//         printf("\n");
//     }
//     
//     free (rdata[0]);                                                                /* Close and release resources.*/
//     free (rdata);
//     status = H5Dclose (dset);
//     status = H5Sclose (space);
//     status = H5Tclose (filetype);
//     status = H5Tclose (memtype);
//     status = H5Fclose (file);
//     return 0;
// }
////////////
// Adapted from example 2 of http://web.mit.edu/fwtools_v3.1.0/www/Intro/IntroExamples.html#CheckAndReadExample
/*    This example reads hyperslab from the SDS.h5 file created by h5_write.c program into two-dimensional
 *   plane of the three-dimensional array.Information about dataset in the SDS.h5 file is obtained. */
#include <hdf5/serial/hdf5.h>
#define FILE        "./particle_files/particles_pos_100000.h5"//"SDS.h5"
#define DATASETNAME "Vec3DF_Array" 
#define NX_SUB  3                       /* hyperslab dimensions */ 
#define NY_SUB  4 
#define NX 7                            /* output buffer dimensions */ 
#define NY 7 
#define NZ  3 
#define RANK         2
#define RANK_OUT     3

int main (void){
    hid_t       file, dataset;          /* handles */
    hid_t       datatype, dataspace;   
    hid_t       memspace; 
    H5T_class_t class_t;                /* datatype class */ /*nb this was a C file*/
    H5T_order_t order;                  /* data order */
    size_t      size;                   /* size of the data element stored in file */
    hsize_t     dimsm[3];               /* memory space dimensions */
    hsize_t     dims_out[2];            /* dataset dimensions */      
    herr_t      status;                             
    int         data_out[NX][NY][NZ ];  /* output buffer */
   
    hsize_t      count[2];              /* size of the hyperslab in the file */
    hsize_t      offset[2];             /* hyperslab offset in the file */
    hsize_t      count_out[3];          /* size of the hyperslab in memory */
    hsize_t      offset_out[3];         /* hyperslab offset in memory */
    int          i, j, k, status_n, rank;

    for (j = 0; j < NX; j++) {
	for (i = 0; i < NY; i++) {
	    for (k = 0; k < NZ ; k++)
		data_out[j][i][k] = 0;
	}
    } 
    file = H5Fopen(FILE, H5F_ACC_RDONLY, H5P_DEFAULT);                      /* Open the file and the dataset.*/
    dataset = H5Dopen(file, DATASETNAME, H5P_DEFAULT);

    /* Get datatype and dataspace handles and then query dataset class, order, size, rank and dimensions.*/
    datatype    = H5Dget_type(dataset);                                     /* datatype handle */ 
    class_t     = H5Tget_class(datatype);
    if (class_t == H5T_FLOAT) printf("Data set has FLOAT type \n");
    order       = H5Tget_order(datatype);
    if (order   == H5T_ORDER_LE) printf("Little endian order \n");

    size  = H5Tget_size(datatype);
    printf(" Data size is %zu \n", size);

    dataspace = H5Dget_space(dataset);                                      /* dataspace handle */
    rank      = H5Sget_simple_extent_ndims(dataspace);
    status_n  = H5Sget_simple_extent_dims(dataspace, dims_out, NULL);
    printf("rank %d, dimensions %lu x %lu \n", rank,
	   (unsigned long)(dims_out[0]), (unsigned long)(dims_out[1]));

    offset[0] = 1;                                                          /* Define hyperslab in the dataset. */
    offset[1] = 2;
    count[0]  = NX_SUB;
    count[1]  = NY_SUB;
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    dimsm[0] = NX;                                                          /* Define the memory dataspace.*/
    dimsm[1] = NY;
    dimsm[2] = NZ ;
    memspace = H5Screate_simple(RANK_OUT,dimsm,NULL);   

    offset_out[0] = 3;                                                      /* Define memory hyperslab. */
    offset_out[1] = 0;
    offset_out[2] = 0;
    count_out[0]  = NX_SUB;
    count_out[1]  = NY_SUB;
    count_out[2]  = 1;
    status = H5Sselect_hyperslab(memspace, H5S_SELECT_SET, offset_out, NULL, count_out, NULL);

    /* Read data from hyperslab in the file into the hyperslab in  memory and display. */
    status = H5Dread(dataset, H5T_NATIVE_INT, memspace, dataspace,
		     H5P_DEFAULT, data_out);
    for (j = 0; j < NX; j++) {
	for (i = 0; i < NY; i++) printf("%d ", data_out[j][i][0]);
	printf("\n");
    }
    /*
     * 0 0 0 0 0 0 0
     * 0 0 0 0 0 0 0
     * 0 0 0 0 0 0 0
     * 3 4 5 6 0 0 0  
     * 4 5 6 7 0 0 0
     * 5 6 7 8 0 0 0
     * 0 0 0 0 0 0 0
     */
    H5Tclose(datatype);                                                     /* Close/release resources. */
    H5Dclose(dataset);
    H5Sclose(dataspace);
    H5Sclose(memspace);
    H5Fclose(file);
    return 0;
}     
