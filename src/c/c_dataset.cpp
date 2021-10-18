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

#include "c_dataset.h"
#include "srexception.h"

using namespace SmartRedis;

// Store an error encountered
void sr_set_last_error(const smart_error& last_error);

// Create a new DataSet.
// The user is responsible for deallocating the DataSet via DeallocateeDataSet()
// Deprecated! This routine will be removed in the next revision of the
// SmartRedis library in favor of the routine below
extern "C"
void* CDataSet(const char* name, const size_t name_length)
{
  // Sanity check parameters
  if (name == NULL)
    return NULL;

  std::string name_str(name, name_length);

  DataSet* dataset = NULL;
  try {
    dataset = new DataSet(name_str);
  } catch (...) {
    dataset = NULL;
  }

  return reinterpret_cast<void* >(dataset);
}

// Create a new DataSet.
// The user is responsible for deallocating the DataSet via DeallocateeDataSet()
extern "C"
SRError CDataSet(const char* name, const size_t name_length, void** new_dataset)
{
  SRError result = sr_ok;
  try {
    // Sanity check params
    SR_CHECK_PARAMS(name != NULL && new_dataset != NULL);

    std::string name_str(name, name_length);
    DataSet* dataset = new DataSet(name_str);
    *new_dataset = reinterpret_cast<void* >(dataset);
  }
  catch (const std::bad_alloc& e) {
    *new_dataset = NULL;
    sr_set_last_error(smart_bad_alloc("client allocation"));
    result = sr_badalloc;
  }
  catch (const smart_error& e) {
    *new_dataset = NULL;
    sr_set_last_error(e);
    result = e.to_error_code();
  }
  catch (...) {
    *new_dataset = NULL;
    sr_set_last_error(smart_unknown_error("Unknown exception occurred"));
    result = sr_unknown;
  }

  return result;
}

// Deallocate a DataSet
// Deprecated! This routine will be removed in the next revision of the
// SmartRedis library in favor of the routine below
extern "C"
void DeallocateeDataSet(void* dataset)
{
  // Sanity check parameters
  if (dataset == NULL)
    return;

  DataSet* d = reinterpret_cast<DataSet*>(dataset);
  delete d;
}

// Deallocate a DataSet
extern "C"
SRError DeallocateeDataSet(void** dataset)
{
  SRError result = sr_ok;
  try
  {
    // Sanity check params
    SR_CHECK_PARAMS(dataset != NULL);

    DataSet* d = reinterpret_cast<DataSet*>(*dataset);
    delete d;
    *dataset = NULL;
  }
  catch (const smart_error& e) {
    sr_set_last_error(e);
    result = e.to_error_code();
  }
  catch (...) {
    sr_set_last_error(smart_unknown_error("Unknown exception occurred"));
    result = sr_unknown;
  }

  return result;
}

// Add a tensor to the dataset.
extern "C"
SRError add_tensor(void* dataset,
                   const char* tensor_name,
                   const size_t tensor_name_length,
                   void* data,
                   const size_t* dims,
                   const size_t n_dims,
                   const CTensorType type,
                   const CMemoryLayout mem_layout)
{
  SRError result = sr_ok;
  try
  {
    // Sanity check params
    SR_CHECK_PARAMS(dataset != NULL && dims != NULL);

    DataSet* d = reinterpret_cast<DataSet*>(dataset);
    std::string tensor_name_str = std::string(tensor_name, tensor_name_length);

    std::vector<size_t> dims_vec;
    dims_vec.assign(dims, dims + n_dims);

    d->add_tensor(tensor_name_str, data, dims_vec,
                  convert_tensor_type(type),
                  convert_layout(mem_layout));
  }
  catch (const smart_error& e) {
    sr_set_last_error(e);
    result = e.to_error_code();
  }
  catch (...) {
    sr_set_last_error(smart_unknown_error("Unknown exception occurred"));
    result = sr_unknown;
  }

  return result;
}

// Add a meta data value to the named meta data field
extern "C"
SRError add_meta_scalar(void* dataset,
                        const char* name,
                        const size_t name_length,
                        const void* data,
                        const CMetaDataType type)
{
  SRError result = sr_ok;
  try
  {
    // Sanity check params
    SR_CHECK_PARAMS(dataset != NULL && name != NULL && data != NULL);

    DataSet* d = reinterpret_cast<DataSet*>(dataset);
    std::string name_str(name, name_length);

    d->add_meta_scalar(name_str, data,
                      convert_metadata_type(type));
  }
   catch (const smart_error& e) {
    sr_set_last_error(e);
    result = e.to_error_code();
  }
  catch (...) {
    sr_set_last_error(smart_unknown_error("Unknown exception occurred"));
    result = sr_unknown;
  }

  return result;
}

// Add a meta data value to the named meta data field
extern "C"
SRError add_meta_string(void* dataset,
                        const char* name,
                        const size_t name_length,
                        const char* data,
                        const size_t data_length)
{
  SRError result = sr_ok;
  try
  {
    // Sanity check params
    SR_CHECK_PARAMS(dataset != NULL && name != NULL && data != NULL);

    DataSet* d = reinterpret_cast<DataSet*>(dataset);
    std::string name_str(name, name_length);
    std::string data_str(data, data_length);

    d->add_meta_string(name_str, data_str);
  }
   catch (const smart_error& e) {
    sr_set_last_error(e);
    result = e.to_error_code();
  }
  catch (...) {
    sr_set_last_error(smart_unknown_error("Unknown exception occurred"));
    result = sr_unknown;
  }

  return result;
}

// Get a tensor of a specified type from the database.
// This function may allocate new memory for the tensor.
// This memory will be deleted when the user deletes the
// DataSet object.
extern "C"
SRError get_dataset_tensor(void* dataset,
                           const char* name,
                           const size_t name_length,
                           void** data,
                           size_t** dims,
                           size_t* n_dims,
                           CTensorType* type,
                           const CMemoryLayout mem_layout)
{
  SRError result = sr_ok;
  try
  {
    // Sanity check params
    SR_CHECK_PARAMS(dataset != NULL && name != NULL && data != NULL) &&
                    dims != NULL && n_dims != NULL && type != NULL);

    DataSet* d = (DataSet* )dataset;
    std::string name_str(name, name_length);

    TensorType t_type = TensorType::undefined;
    d->get_tensor(name_str,* data,* dims,* n_dims,
                  t_type, convert_layout(mem_layout));
    *type = convert_tensor_type(t_type);
  }
  catch (const smart_error& e) {
    sr_set_last_error(e);
    result = e.to_error_code();
  }
  catch (...) {
    sr_set_last_error(smart_unknown_error("Unknown exception occurred"));
    result = sr_unknown;
  }

  return result;
}

// Copy the tensor data buffer into the provided memory space (data).
extern "C"
SRError unpack_dataset_tensor(void* dataset,
                              const char* name,
                              const size_t name_length,
                              void* data,
                              const size_t* dims,
                              const size_t n_dims,
                              const CTensorType type,
                              const CMemoryLayout mem_layout)
{
  SRError result = sr_ok;
  try
  {
    // Sanity check params
    SR_CHECK_PARAMS(dataset != NULL && name != NULL && dims != NULL);

    DataSet* d = reinterpret_cast<DataSet*>(dataset);
    std::string name_str(name, name_length);

    std::vector<size_t> dims_vec;
    dims_vec.assign(dims, dims + n_dims);

    d->unpack_tensor(name_str, data, dims_vec,
                    convert_tensor_type(type),
                    convert_layout(mem_layout));
  }
  catch (const smart_error& e) {
    sr_set_last_error(e);
    result = e.to_error_code();
  }
  catch (...) {
    sr_set_last_error(smart_unknown_error("Unknown exception occurred"));
    result = sr_unknown;
  }

  return result;
}

// Get a meta data field. This method may allocate memory that is cleared when
// the user deletes the DataSet object
// Deprecated! This routine will be removed in the next revision of the
// SmartRedis library in favor of the routine below
extern "C"
void* get_meta_scalars(void* dataset,
                       const char* name,
                       const size_t name_length,
                       size_t* length,
                       CMetaDataType* type)
{
  // Sanity check parameters
  if (dataset == NULL || name == NULL || length == NULL || type == NULL)
    return NULL;

  DataSet* d = reinterpret_cast<DataSet*>(dataset);
  std::string key_str(name, name_length);
  void* data = NULL;
  MetaDataType m_type;

  d->get_meta_scalars(key_str, data, *length, m_type);

  *type = convert_metadata_type(m_type);
  return data;
}

// Get a meta data field. This method may allocate memory that is cleared when
// the user deletes the DataSet object
extern "C"
SRError get_meta_scalars(void* dataset,
                       const char* name,
                       const size_t name_length,
                       size_t* length,
                       CMetaDataType* type,
                       void** scalar_data)
{
  SRError result = sr_ok;
  try
  {
    // Sanity check params
    SR_CHECK_PARAMS(dataset != NULL && name != NULL && length != NULL &&
                    type != NULL && scalars != NULL);

    DataSet* d = reinterpret_cast<DataSet*>(dataset);
    std::string key_str(name, name_length);

    void* data = NULL;
    MetaDataType m_type;
    d->get_meta_scalars(key_str, data, *length, m_type);

    *type = convert_metadata_type(m_type);
    *scalar_data = data;
  }
  catch (const smart_error& e) {
    sr_set_last_error(e);
    result = e.to_error_code();
  }
  catch (...) {
    sr_set_last_error(smart_unknown_error("Unknown exception occurred"));
    result = sr_unknown;
  }

  return result;
}

// Get a meta data string field. This method may
// allocate memory that is cleared when the user
// deletes the DataSet object.
extern "C"
SRError get_meta_strings(void* dataset,
                         const char* name,
                         const size_t name_length,
                         char*** data,
                         size_t* n_strings,
                         size_t** lengths)
{
  SRError result = sr_ok;
  try
  {
    // Sanity check params
    SR_CHECK_PARAMS(dataset != NULL && name != NULL && data != NULL &&
                    n_strings != NULL && lengths != NULL);

    DataSet* d = reinterpret_cast<DataSet*>(dataset);
    std::string key_str(name, name_length);
    d->get_meta_strings(key_str, *data, *n_strings, *lengths);
  }
  catch (const smart_error& e) {
    sr_set_last_error(e);
    result = e.to_error_code();
  }
  catch (...) {
    sr_set_last_error(smart_unknown_error("Unknown exception occurred"));
    result = sr_unknown;
  }

  return result;
}
