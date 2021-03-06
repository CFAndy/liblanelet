// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
// Copyright (c) 2017, FZI Forschungszentrum Informatik
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!\file
 *
 * \author  Florian Kuhnt <kuhnt@fzi.de>
 * \author  Tobias Fleck <tfleck@fzi.de>
 * \date    2016-06-03
 *
 */
//----------------------------------------------------------------------

#include <iostream>

#include <icl_core_config/Config.h>

#include <liblanelet/LaneletMap.hpp>
#include <liblanelet/LaneletGraph.hpp>

#include <boost/graph/graphviz.hpp>


template <typename Graph>
class VertexWriter {
public:
    VertexWriter(Graph const & graph) : m_graph(graph) {

    }

    template<typename Vertex>
    void operator()(std::ostream& out, Vertex v) const
    {
      out << "[label=\"" << (m_graph[v].lanelet)->id()<< "\"]";
    }
private:
  const Graph & m_graph;
};



int main(int argc, char ** argv)
{
  using namespace icl_core;

  //define parameter
  config::addParameter(config::ConfigPositionalParameter("lanelet_map",
                                                         "/lanelet_map",
                                                         "The source lanelet file we want to read from.\n Defaults to \"/tmp/map.osm\".", true));

  config::addParameter(config::ConfigPositionalParameter("output_file",
                                                         "/output_file",
                                                         "The output graphviz file we want to write the graph to.\n Defaults to \"/tmp/graph.dot\".", true));



  //Initialize logger.
  icl_core::logging::initialize(argc, argv);

  //Get the parameter.
  std::string path_to_lanelet_map = config::getDefault<std::string>("/lanelet_map", "/tmp/map.osm");
  std::string path_to_output = config::getDefault<std::string>("/output_file", "/tmp/graph.dot");

  std::cout << "Reading Lanelet from file:" << std::endl;
  std::cout << path_to_lanelet_map << std::endl;
  std::cout << "Converting Lanelet..." << std::endl;


  //Load the lanelet-map.
  LLet::LaneletMap map(path_to_lanelet_map, true); // ignore_consistency_failures = true

  std::cout << "The graph contains " << map.lanelets().size() << " lanelets." << std::endl;
  LLet::Graph graph = map.graph();

  //out-stream for the file to write.
  std::ofstream os;
  os.open(path_to_output);

  if(!os.is_open())
  {
    std::cout << "Failed to write file:" << std::endl
              << path_to_output << std::endl
              << "Finishing program..." << std::endl;
    os.close();
    return -42;
  }




  boost::write_graphviz(os, graph, VertexWriter<LLet::Graph>(graph));


  std::cout << "Writing graph to file" << std::endl;
  std::cout << path_to_output << std::endl;
  std::cout << "Writing Successful!" << std::endl;
  //close the filestream.
  os.close();
  return 0;
}
