#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>


#include "BloomFilter.hpp"
#include "utils/Helpers.hpp"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    po::options_description desc;
    po::variables_map vm;
    desc.add_options()
        ("file-with-saved-data", po::value<std::string>()->required(), "Path to file with data to save in bloom filter")
        ("hash-size", po::value<uint32_t>()->required(), "Hash size for bloom filter")
        ("hash-count", po::value<uint32_t>()->required(), "Hash count for bloom filter")
        ("hash-start", po::value<uint32_t>()->default_value(0), "Start using hashes from bit");

    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }
    catch (const po::error &e)
    {
        desc.print(std::cerr);
        throw std::runtime_error(e.what());
    }

    auto hashSize = vm["hash-size"].as<uint32_t>();
    auto hashCount = vm["hash-count"].as<uint32_t>();
    auto hashStart = vm["hash-start"].as<uint32_t>();
    auto fileName = vm["file-with-saved-data"].as<std::string>();

    hobf::BloomFilter bf(vm["hash-size"].as<uint32_t>(), vm["hash-count"].as<uint32_t>(), vm["hash-start"].as<uint32_t>());

    std::ifstream dataStream(vm["file-with-saved-data"].as<std::string>(), std::ios::in);
    if (!dataStream)
    {
        throw std::runtime_error("Could not open file with saved data '" + fileName + "'");
    }


    auto toCompute = (hashStart + hobf::utils::getPower(hashSize) * hashCount + 127) / 128;

    std::vector<uint32_t> seeds(toCompute);
    for (uint32_t i = 0; i < seeds.size(); ++i)
    {
        seeds[i] = i + 1;
    }

    std::string elem;
    std::vector<uint64_t> hashes;
    hashes.reserve(seeds.size() * 2);

    while(std::getline(dataStream, elem))
    {
        hashes.clear();
        for (auto seed: seeds)
        {
            auto hash = hobf::utils::hash(elem, seed);
            hashes.push_back(hash.first);
            hashes.push_back(hash.second);
        }
        bf.add(hashes);
    }

    while(std::getline(std::cin, elem))
    {
        hashes.clear();
        for (auto seed: seeds)
        {
            auto hash = hobf::utils::hash(elem, seed);
            hashes.push_back(hash.first);
            hashes.push_back(hash.second);
        }
        if (bf.lookup(hashes))
        {
            std::cout << elem << "\n";
        }
    }
}