#pragma once

#include <algorithm>
#include <vector>

struct SparseProjection
{
    unsigned int *indInG;
    unsigned int *ind;
    unsigned int *preInd;
    unsigned int *revIndInG;
    unsigned int *revInd;
    unsigned int *remap;
    unsigned int connN;
};

template<typename Generator>
void build_fixed_probability_connector(unsigned int numPre, unsigned int numPost, float probability,
                                       SparseProjection &projection, bool plastic, Generator &gen)
{
  // Allocate memory for indices
  projection.indInG = new unsigned int[numPre + 1];

  // Reserve a temporary vector to store indices
  std::vector<unsigned int> tempInd;
  tempInd.reserve((unsigned int)((float)(numPre * numPost) * probability));

  // Create RNG to draw probabilities
  std::uniform_real_distribution<> dis(0.0, 1.0);

  // Loop through pre neurons
  for(unsigned int i = 0; i < numPre; i++)
  {
    // Connections from this neuron start at current end of indices
    projection.indInG[i] = tempInd.size();

    // Loop through post neurons
    for(unsigned int j = 0; j < numPost; j++)
    {
      // If there should be a connection here, add one to temporary array
      if(dis(gen) < probability)
      {
        tempInd.push_back(j);
      }
    }
  }

  projection.connN = tempInd.size();
  projection.indInG[numPre] = projection.connN;


  // Allocate memory for indices and copy in temporary indices
  projection.ind = new unsigned int[projection.connN];
  std::copy(tempInd.begin(), tempInd.end(), &projection.ind[0]);

  // If connection is plastic, allocate extra data
  // structures to allow lookup by postsynaptic index
  if(plastic)
  {
    projection.revIndInG = new unsigned int[numPost + 1];
    projection.revInd= new unsigned int[projection.connN];
    projection.remap= new unsigned int[projection.connN];
  }
}