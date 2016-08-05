/**
 *	@file DecoderRing.h
 *  @author Matthew Chan
 *  @date 7/19/2016
 *	@version 0.1
 *  @class DecoderRing
 *  @brief This class generates a decoder ring using two seeds for std::default_random_engine
 */
#ifndef DECODERRING_H
#define DECODERRING_H

#include <random>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>

class DecoderRing {
public:
	DecoderRing(unsigned int seed1, unsigned int seed2);
	unsigned int getValue(unsigned int i);
	
private:
	std::vector<unsigned int> inner;
	std::vector<unsigned int> outer;
	std::default_random_engine engine;
	std::map<unsigned int, unsigned int> map;
};

#endif