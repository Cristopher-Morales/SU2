 /*!
 * \file CDeformationDriver.hpp
 * \brief Headers of the main subroutines for driving the mesh deformation.
 * \author T. Economon, H. Kline, R. Sanchez
 * \version 7.1.1 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2021, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../../../Common/include/parallelization/mpi_structure.hpp"

#include "../../../Common/include/grid_movement/CSurfaceMovement.hpp"
#include "../../../Common/include/grid_movement/CVolumetricMovement.hpp"
#include "../../../SU2_CFD/include/output/COutput.hpp"
#include "../../../SU2_CFD/include/numerics/CNumerics.hpp"
#include "../../../Common/include/geometry/CGeometry.hpp"

/*!
 * \class CDeformationDriver
 * \brief Class for driving mesh deformation solvers.
 * \author A. Gastaldi, H. Patel
 * \version 7.1.1 "Blackbird"
 */
class CDeformationDriver {
protected:
  char config_file_name[MAX_STRING_SIZE];
  int rank,
      size;
  su2double StartTime,                          /*!< \brief Start point of the timer for performance benchmarking.*/
            StopTime,                           /*!< \brief Stop point of the timer for performance benchmarking.*/
            UsedTimePreproc,                    /*!< \brief Elapsed time between Start and Stop point of the timer for tracking preprocessing phase.*/
            UsedTimeCompute,                    /*!< \brief Elapsed time between Start and Stop point of the timer for tracking compute phase.*/
            UsedTime;                           /*!< \brief Elapsed time between Start and Stop point of the timer.*/
  unsigned short nDim;
  unsigned short iZone, nZone = SINGLE_ZONE;
  CConfig *driver_config;                       /*!< \brief Definition of the driver configuration. */
  CConfig **config_container;                   /*!< \brief Definition of the particular problem. */
  CGeometry **geometry_container;             /*!< \brief Geometrical definition of the problem. */
  CSurfaceMovement **surface_movement;          /*!< \brief Surface movement classes of the problem. */
  CVolumetricMovement **grid_movement;         /*!< \brief Volume grid movement classes of the problem. */
  CSolver **solver_container;
  CNumerics ***numerics_container;
  COutput **output_container;                   /*!< \brief Pointer to the COutput class. */

public:
  /*!
   * \brief Constructor of the class.
   * \param[in] confFile - Configuration file name.
   * \param[in] MPICommunicator - MPI communicator for SU2.
   */
  CDeformationDriver(char* confFile, SU2_Comm MPICommunicator);

  /*!
   * \brief Destructor of the class.
   */
  ~CDeformationDriver(void);

  /*!
   * \brief [Overload] Launch the computation for single-zone problems.
   */
  void Run();

  /*!
   * \brief Output the mesh.
   */
  void Output();

  /*!
   * \brief Deallocation routine
   */
  void Postprocessing();

  /*!
   * \brief Get all the deformable boundary marker tags.
   * \return List of deformable boundary markers tags.
   */
  vector<string> GetAllDeformMeshMarkersTag() const;

  /*!
   * \brief Get all the boundary markers tags with their associated indices.
   * \return List of boundary markers tags with their indices.
   */
  map<string, int> GetAllBoundaryMarkers() const;

  /*!
   * \brief Get all the boundary markers tags with their associated types.
   * \return List of boundary markers tags with their types.
   */
  map<string, string> GetAllBoundaryMarkersType() const;

  /*!
   * \brief Check if a vertex is physical or not (halo node) on a specified marker.
   * \param[in] iMarker - Marker identifier.
   * \param[in] iVertex - Vertex identifier.
   * \return True if the specified vertex is a halo node.
   */
  bool IsAHaloNode(unsigned short iMarker, unsigned long iVertex) const;

  /*!
   * \brief Get the global index of a vertex on a specified marker.
   * \param[in] iMarker - Marker identifier.
   * \param[in] iVertex - Vertex identifier.
   * \return Vertex global index.
   */
  unsigned long GetVertexGlobalIndex(unsigned short iMarker, unsigned long iVertex) const;

  /*!
   * \brief Get undeformed coordinates from the mesh solver.
   * \param[in] iMarker - Marker identifier.
   * \param[in] iVertex - Vertex identifier.
   * \return x,y,z coordinates of the vertex.
   */
  vector<passivedouble> GetInitialMeshCoord(unsigned short iMarker, unsigned long iVertex) const;

  /*!
   * \brief Get the unit normal (vector) at a vertex on a specified marker.
   * \param[in] iMarker - Marker identifier.
   * \param[in] iVertex - Vertex identifier.
   * \return Unit normal (vector) at the vertex.
   */
  vector<passivedouble> GetVertexNormal(unsigned short iMarker, unsigned long iVertex, bool unitNormal = false) const;

  inline vector<passivedouble> GetVertexUnitNormal(unsigned short iMarker, unsigned long iVertex) const {
    return GetVertexNormal(iMarker, iVertex, true);
  }

  /*!
   * \brief Get the number of mesh dimensions.
   * \return Number of dimensions.
   */
  unsigned long GetNumberDimensions() const;
   
  /*!
   * \brief Get the number of mesh elements.
   * \return Number of elements.
   */
  unsigned long GetNumberElements() const;
  
  /*!
   * \brief Get the number of mesh elements from a specified marker.
   * \param[in] iMarker - Marker identifier.
   * \return Number of elements.
   */
  unsigned long GetNumberElementsMarker(unsigned short iMarker) const;
  
  /*!
   * \brief Get the number of mesh vertices.
   * \return Number of vertices.
   */
  unsigned long GetNumberVertices() const;
  
  /*!
   * \brief Get the number of mesh vertices from a specified marker.
   * \param[in] iMarker - Marker identifier.
   * \return Number of vertices.
   */
  unsigned long GetNumberVerticesMarker(unsigned short iMarker) const;
  
  /*!
   * \brief Get the number of halo mesh vertices.
   * \return Number of vertices.
   */
  unsigned long GetNumberHaloVertices() const;

  /*!
   * \brief Get the number of halo mesh vertices from a specified marker.
   * \param[in] iMarker - Marker identifier.
   * \return Number of vertices.
   */
  unsigned long GetNumberHaloVerticesMarker(unsigned short iMarker) const;
  
  /*!
   * \brief Get global IDs of mesh vertices.
   * \return Global vertex IDs.
   */
  vector<unsigned long> GetVertexIDs() const;
  
  /*!
   * \brief Get global IDs of mesh vertices.
   * \param[in] iMarker - Marker identifier.
   * \return Global vertex IDs.
   */
  vector<unsigned long> GetVertexIDsMarker(unsigned short iMarker) const;
  
  /*!
   * \brief Get global IDs of mesh elements.
   * \return Global element IDs.
   */
  vector<unsigned long> GetElementIDs() const;
  
  /*!
   * \brief Get global IDs of mesh elements.
   * \param[in] iMarker - Marker identifier.
   * \return Global element IDs.
   */
  vector<unsigned long> GetElementIDsMarker(unsigned short iMarker) const;
  
  /*!
   * \brief Get the connected point IDs of mesh elements.
   * \return Element connectivities (nElem, nNode)
   */
  vector<vector<unsigned long>> GetConnectivity() const;
  
  /*!
   * \brief Get the connected point IDs of mesh elements on a specified marker.
   * \param[in] iMarker - Marker identifier.
   * \return Element connectivities (nBound, nNode).
   */
  vector<vector<unsigned long>> GetConnectivityMarker(unsigned short iMarker) const;
    
  /*!
   * \brief Get halo node stauts of mesh vertices. 
   * \return Point domain status.
   */
  vector<bool> GetDomain() const;

  /*!
   * \brief Get halo node stauts of mesh marker vertices. 
   * \param[in] iMarker - Marker identifier.
   * \return Point domain status.
   */
  vector<bool> GetDomainMarker(unsigned short iMarker) const;

  /*!
   * \brief Get the coordinates of the mesh points.
   * \return Point coordinates (nPoint*nDim).
   */
  vector<passivedouble> GetCoordinates() const;
  
  /*!
   * \brief Get the coordinates of the mesh points on the specified marker.
   * \param[in] iMarker - Marker identifier.
   * \return Point coordinates (nVertex*nDim).
   */
  vector<passivedouble> GetCoordinatesMarker(unsigned short iMarker) const;
  
  /*!
   * \brief Set the coordinates of the mesh points [DEBUGGING].
   * \param[in] values - Point coordinates (nPoint*nDim).
   */
  void SetCoordinates(vector<passivedouble> values);
  
  /*!
   * \brief Set the coordinates of the mesh points on the specified marker [DEBUGGING].
   * \param[in] iMarker - Marker identifier.
   * \param[in] values - Point coordinates (nVertex*nDim).
   */
  void SetCoordinatesMarker(unsigned short iMarker, vector<passivedouble> values);
  
  /*!
   * \brief Get the vertex displacements on the specified marker.
   * \param[in] iMarker - Marker identifier.
   * \return Vertex displacements (nVertex*nDim).
   */
  vector<passivedouble> GetDisplacementsMarker(unsigned short iMarker) const;
  
  /*!
   * \brief Set the vertex displacements on the specified marker.
   * \param[in] iMarker - Marker identifier.
   * \param[in] values - Vertex displacements (nVertex*nDim).
   */
  void SetDisplacementsMarker(unsigned short iMarker, vector<passivedouble> values);
  
  /*!
   * \brief Get the vertex velocities on the specified marker.
   * \param[in] iMarker - Marker identifier.
   * \return Vertex velocities (nVertex*nDim).
   */
  vector<passivedouble> GetVelocitiesMarker(unsigned short iMarker) const;
  
  /*!
   * \brief Set the vertex velocities on the specified marker.
   * \param[in] iMarker - Marker identifier.
   * \param[in] values - Vertex velocities (nVertex*nDim).
   */
  void SetVelocitiesMarker(unsigned short iMarker, vector<passivedouble> values);
  
  /*!
   * \brief Set the x coordinate of a vertex on a specified marker.
   * \param[in] iMarker - Marker identifier.
   * \param[in] iVertex - Vertex identifier.
   * \param[in] newPosX - New x coordinate of the vertex.
   */
  void SetVertexCoordX(unsigned short iMarker, unsigned long iVertex, passivedouble newPosX);

  /*!
   * \brief Set the y coordinate of a vertex on a specified marker.
   * \param[in] iMarker - Marker identifier.
   * \param[in] iVertex - Vertex identifier.
   * \param[in] newPosY - New y coordinate of the vertex.
   */
  void SetVertexCoordY(unsigned short iMarker, unsigned long iVertex, passivedouble newPosY);

  /*!
   * \brief Set the z coordinate of a vertex on a specified marker.
   * \param[in] iMarker - Marker identifier.
   * \param[in] iVertex - Vertex identifier.
   * \param[in] newPosZ - New z coordinate of the vertex.
   */
  void SetVertexCoordZ(unsigned short iMarker, unsigned long iVertex, passivedouble newPosZ);

  /*!
   * \brief Get the vertex displacements on the specified marker.
   * \param[in] iMarker - Marker identifier.
   * \return Vertex displacements (nVertex, nDim).
   */
  vector<passivedouble> GetMeshDisplacementsMarker(unsigned short iMarker) const;

  /*!
   * \brief Set the vertex displacements on the specified marker.
   * \param[in] iMarker - Marker identifier.
   * \param[in] values - Vertex displacements (nVertex, nDim).
   */
  void SetMeshDisplacementsMarker(unsigned short iMarker, vector<passivedouble> values);

  /*!
   * \brief Communicate the boundary mesh displacements in a python call
   */
  void CommunicateMeshDisplacement(void);

protected:
  /*!
   * \brief Init_Containers
   */
  void SetContainers_Null();

  /*!
   * \brief Read in the config and mesh files.
   */
  void Input_Preprocessing();

  /*!
   * \brief Construction of the edge-based data structure.
   */
  void Geometrical_Preprocessing();

  /*!
   * \brief Preprocess the output container.
   */
  void Output_Preprocessing();

  /*!
   * \brief Preprocess the mesh solver container.
   */
  void Solver_Preprocessing();

  /*!
   * \brief Preprocess the numerics container.
   */
  void Numerics_Preprocessing();

  /*!
   * \brief Mesh deformation based on linear elasticity solver (CMeshSolver).
   */
  void Update();

  /*!
   * \brief Mesh deformation based on legacy implementation.
   */
  void Update_Legacy();

};
