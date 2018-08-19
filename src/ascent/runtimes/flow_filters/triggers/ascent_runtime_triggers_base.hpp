//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2015-2018, Lawrence Livermore National Security, LLC.
// 
// Produced at the Lawrence Livermore National Laboratory
// 
// LLNL-CODE-716457
// 
// All rights reserved.
// 
// This file is part of Ascent. 
// 
// For details, see: http://ascent.readthedocs.io/.
// 
// Please also read ascent/LICENSE
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the disclaimer below.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the disclaimer (as noted below) in the
//   documentation and/or other materials provided with the distribution.
// 
// * Neither the name of the LLNS/LLNL nor the names of its contributors may
//   be used to endorse or promote products derived from this software without
//   specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY,
// LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


//-----------------------------------------------------------------------------
///
/// file: ascent_runtime_trigger_base.hpp
///
//-----------------------------------------------------------------------------

#ifndef ASCENT_RUNTIME_TRIGGER_BASE
#define ASCENT_RUNTIME_TRIGGER_BASE

#include <ascent.hpp>

#include <flow_filter.hpp>


//-----------------------------------------------------------------------------
// -- begin ascent:: --
//-----------------------------------------------------------------------------
namespace ascent
{

//-----------------------------------------------------------------------------
// -- begin ascent::runtime --
//-----------------------------------------------------------------------------
namespace runtime
{

//-----------------------------------------------------------------------------
// -- begin ascent::runtime::filters --
//-----------------------------------------------------------------------------
namespace filters
{

//-----------------------------------------------------------------------------
///
/// Trigger Filters
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class TriggerFilter : public ::flow::Filter
{
private:
    TriggerFilter();
    const bool m_is_field_trigger;
    const bool m_is_state_trigger;
    const bool m_is_topology_trigger;

    std::string get_topology_name();
public:

    TriggerFilter(bool is_field, bool is_state, bool is_topo);
    virtual ~TriggerFilter();
    
    bool verify_params(const conduit::Node &params,
                       conduit::Node &info);
    virtual void   declare_interface(conduit::Node &i);
    virtual void   execute();
protected:
    virtual bool trigger() = 0;
    // GetTypeName: unique typename that is used to create the filter the runtime
    virtual std::string  get_type_name() = 0; 

    const conduit::Node &get_topology();
    const conduit::Node &get_field();
    const conduit::Node &get_state();
    const conduit::Node &get_coords();
};



struct StateContainer
{
  std::map<std::string, conduit::Node*> m_states;
  ~StateContainer()
  {
    std::cout<<"DELETING STATES\n";
    for(auto it = m_states.begin(); it != m_states.end(); ++it)
    {
      delete m_states[it->first];
    }
  }
};

class State
{
public:
  // sets the state of a given key and assumes ownership of the pointer.
  // if a state already exists, it is deleted and replaced by the new state.
  void set_state(const std::string &key, conduit::Node *state);
  // retrieves the state of the given key. If no state exists, nullptr is returned
  conduit::Node* get_state(const std::string &key);
  // returns true of the state exists 
  bool has_state(const std::string &key);
private:
  static StateContainer m_state_container;
};

//-----------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
// -- end ascent::runtime::filters --
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
// -- end ascent::runtime --
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
// -- end ascent:: --
//-----------------------------------------------------------------------------




#endif
//-----------------------------------------------------------------------------
// -- end header ifdef guard
//-----------------------------------------------------------------------------
