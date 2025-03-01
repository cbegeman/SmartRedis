
/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2021, Hewlett Packard Enterprise
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SMARTREDIS_C_DATASET_H
#define SMARTREDIS_C_DATASET_H
///@file
///\brief C-wrappers for the C++ DataSet class
#include "dataset.h"
#include "sr_enums.h"
#include "srexception.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
*   \brief C-DataSet constructor
*   \param name The name of the dataset
*   \param name_length The length of the dataset name c-string,
*                      excluding null terminating character
*   \param new_dataset Receives the new dataset
*   \return Returns SRNoError on success or an error code on failure
*/
SRError CDataSet(const char* name,
                 const size_t name_length,
                 void** new_dataset);

/*!
*   \brief C-DataSet destructor
*   \param dataset A pointer to the dataset to release
*   \return Returns SRNoError on success or an error code on failure
*/
SRError DeallocateeDataSet(void** dataset);

/*!
*   \brief Add a tensor to the DataSet.
*   \param dataset A c-ptr to the dataset object
*   \param name The name used to reference the tensor within the DataSet
*   \param name_length The length of the dataset name c-string,
*                      excluding null terminating character
*   \param data A c-ptr to the tensor data memory location
*   \param dims The dimensions of the tensor
*   \param n_dims The number of dimensions of the tensor
*   \param type The data type of the tensor data
*   \param mem_layout Memory layout of the provided tensor data
*   \return Returns SRNoError on success or an error code on failure
*/
SRError add_tensor(void* dataset,
                   const char* name,
                   const size_t name_length,
                   void* data,
                   const size_t* dims,
                   const size_t n_dims,
                   const SRTensorType type,
                   const SRMemoryLayout mem_layout);

/*!
*   \brief Add metadata scalar field (non-string) with value to the DataSet.  If
*          the field does not exist, it will be created.
*          If the field exists, the value will be appended to existing field.
*   \param dataset A c-ptr to the dataset object
*   \param name The name used to reference the metadata field
*   \param name_length The length of the dataset name c-string,
*                      excluding null terminating character
*   \param data A c-ptr to the metadata field data
*   \param type The data type of the metadata
*   \return Returns SRNoError on success or an error code on failure
*/
SRError add_meta_scalar(void* dataset,
                        const char* name,
                        const size_t name_length,
                        const void* data,
                        const SRMetaDataType type);


/*!
*   \brief Add metadata string field with value to the DataSet.  If the field
*          does not exist, it will be created.
*          If the field exists the value will be appended to existing field.
*   \param dataset A c-ptr to the dataset object
*   \param name The name used to reference the metadata field
*   \param name_length The length of the dataset name c-string,
*                      excluding null terminating character
*   \param data The string to add to the field
*   \param data_length The length of the metadata string value
*   \return Returns SRNoError on success or an error code on failure
*/
SRError add_meta_string(void* dataset,
                        const char* name,
                        const size_t name_length,
                        const char* data,
                        const size_t data_length);


/*!
*   \brief Get the tensor data, dimensions, and type for the tensor in the
*          dataset. This function will allocate and retain management of the
*          memory for the tensor data.
*   \details The memory of the data pointer is valid until the dataset is
*            destroyed. This method is meant to be used when the dimensions
*            and type of the tensor are unknown or the user does not want to
*            manage memory.  However, given that the memory associated with the
*            return data is valid until dataset destruction, this method should
*            not be used repeatedly for large tensor data.  Instead  it is
*            recommended that the user use unpack_tensor() for large tensor
*            data and to limit memory use by the dataset.
*   \param dataset A c-ptr to the dataset object
*   \param name The name used to reference the tensor in the dataset
*   \param name_length The length of the dataset name c-string,
*                      excluding null terminating character
*   \param data A c-ptr reference that will be pointed to newly allocated memory
*   \param dims The dimensions vector that will be filled with the retrieved
*               tensor dimensions
*   \param n_dims The number of dimensions of the tensor
*   \param type Receives the retrieved tensor type
*   \param mem_layout The layout the newly allocated memory should conform to
*   \return Returns SRNoError on success or an error code on failure
*/
SRError get_dataset_tensor(void* dataset,
                           const char* name,
                           const size_t name_length,
                           void** data,
                           size_t** dims,
                           size_t* n_dims,
                           SRTensorType* type,
                           const SRMemoryLayout mem_layout);

/*!
*   \brief Get tensor data and fill an already allocated array memory space
*          that has the specified MemoryLayout.  The provided type and
*          dimensions are checked against retrieved values to ensure
*          the provided memory space is sufficient.  This method is the most
*          memory efficient way to retrieve tensor data from a dataset.
*   \param dataset A c-ptr to the dataset object
*   \param name The name used to reference the tensor in the dataset
*   \param name_length The length of the dataset name c-string,
*                      excluding null terminating character
*   \param data A c-ptr to the memory space to be filled with tensor data
*   \param dims The dimensions of the memory space
*   \param n_dims The number of dimensions of the tensor in the memory space
*   \param type The TensorType matching the data type of the memory space
*   \param mem_layout The MemoryLayout of the provided memory space.
*   \return Returns SRNoError on success or an error code on failure
*/
SRError unpack_dataset_tensor(void* dataset,
                              const char* name,
                              const size_t name_length,
                              void* data,
                              const size_t* dims,
                              const size_t n_dims,
                              const SRTensorType type,
                              const SRMemoryLayout mem_layout);

/*!
*   \brief Get the metadata scalar field values from the DataSet.  The data
*          pointer reference will be pointed to newly allocated memory that
*          will contain all values in the metadata field. The length variable
*          will be set to the number of entries in the allocated memory space
*          to allow for iteration over the values.  The TensorType enum will
*          be set to the type of the MetaData field.
*   \param dataset A c-ptr to the dataset object
*   \param name The name used to reference the metadata field in the DataSet
*   \param name_length The length of the dataset name c-string,
*                      excluding null terminating character
*   \param length The number of values in the metadata field
*   \param type The data type of the metadata field
*   \param scalar_data Receives allocated memory containing the metadata
*   \return Returns SRNoError on success or an error code on failure
*/
SRError get_meta_scalars(void* dataset,
                         const char* name,
                         const size_t name_length,
                         size_t* length,
                         SRMetaDataType* type,
                         void** scalar_data);


/*!
*   \brief Get the metadata string field values from the dataset.  The data
*          pointer reference will be pointed to newly allocated memory that
*          will contain all values in the metadata field. The n_strings input
*          variable reference will be set to the number of strings in the
*          field.  The lengths c-ptr variable will be pointed to a new memory
*          space that contains the length of each field string.  The memory
*          for the data and lengths pointers will be managed by the DataSet
*          object and remain valid until the DataSet object is destroyed.
*   \param dataset A c-ptr to the dataset object
*   \param name The name used to reference the metadata field in the DataSet
*   \param name_length The length of the dataset name c-string,
*                      excluding null terminating character
*   \param data Receives an array of string values
*   \param n_strings The number of strings returned in \p data
*   \param lengths Recieves an array containing the lengths of the strings
*                  returned in \p data
*   \return Returns SRNoError on success or an error code on failure
*/
SRError get_meta_strings(void* dataset,
                         const char* name,
                         const size_t name_length,
                         char*** data,
                         size_t* n_strings,
                         size_t** lengths);

#ifdef __cplusplus
}
#endif
#endif //SMARTREDIS_C_DATASET_H
