#include "network.h"
#include "random.h"
#include <algorithm>
#include <iostream>
#include <iterator>

void Network::resize(const size_t& n) {
	if (n <= 0) return;
	values.resize(n);
	RNG.normal(values);
	/*! On utilise la fonction resize définie dans la bibliothèque vector */
}

bool Network::add_link(const size_t& a, const size_t& b) {
	
	if ( (a!=b) and (a<values.size()) and (b<values.size()) ) { /*! we make sure we're not handling 2 same nodes and, if not, a and b are part of "values" */
	
		for (auto link:links) {
			if (((link.first == a) and (link.second==b)) or ((link.first==b) and (link.second ==b))) return false; /*! We make sure a link was not previoulsy made between a and b */
		}
		
		links.insert(std::pair<size_t,size_t>(a,b));
		links.insert(std::pair<size_t,size_t>(b,a));
		
		return true;
		
	}
	return false;
}


/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.

  All previous links are cleared first.
  @param mean_deg the average of the Poisson distribution.
  @param[in] mean_deg the average of the Poisson distribution,
  @param[out] the number of links created.
 */
size_t Network::random_connect(const double& mean_deg) {
	links.clear();
	int count(0);
	for (size_t i(0); i<values.size(); ++i) {
		size_t degree_i;
		degree_i = RNG.poisson(mean_deg); /*! no need to test the degree obtained because if it's too big add_link won't allow supplementary links to be created */
		for (size_t j(0); j < degree_i; ++j) {
			while (not add_link(i, RNG.uniform_double(0, values.size()-1))) continue; /*! we want a link to be created necessarily so we try to create one until it works. Not working would be almost impossible when manipulating great numbers of nodes due to the random generator used at each loop. */
			count ++; /*! at this point a link has inevitably been made */
		}
	}
	return count;
}

size_t Network::set_values(const std::vector<double>& v) {
	size_t smaller_size = std::min(v.size(), values.size());
	for (size_t i(0); i < smaller_size; ++i) {
		values[i] = v[i];
	}
	return smaller_size;
}

size_t Network::size() const {
	return values.size();
}

size_t Network::degree(const size_t & n) const {
	if (n<values.size()) {
		return links.count(n);
	} else return 0;
}

double Network::value(const size_t & n) const {
	if (n<values.size()) return values[n];
	else return 0;
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> to_sort = values;
	std::sort(to_sort.begin(), to_sort.end(), std::greater<double>());
	return to_sort;
}

std::vector<size_t> Network::neighbors(const size_t& n) const {
	std::vector<size_t> neighbors;
	for (auto link:links) {
		if ( link.first == n ) neighbors.push_back(link.second);
	}
	return neighbors;
}



