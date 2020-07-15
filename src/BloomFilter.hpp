#pragma once

#include <string>
#include <vector>

namespace hobf
{
    class BloomFilter final
    {
    public:
        BloomFilter(uint32_t hashSize,
            uint32_t hashCount,
            uint32_t startFrom,
            std::vector<std::vector<uint64_t>> &&filters = {})
            : hashSize_(hashSize),
              hashCount_(hashCount),
              startFrom_(startFrom),
              filters_(std::move(filters))
        {
            try
            {
                if (!hashCount_)
                {
                    throw std::runtime_error("hashCount should be positive");
                }
                if (!hashSize_)
                {
                    throw std::runtime_error("hashSize should be positive");
                }
                if (hashSize_ & (hashSize_ - 1))
                {
                    throw std::runtime_error("hashSize should be power of two");
                }
            }
            catch (const std::runtime_error &r)
            {
                throw std::runtime_error(std::string("Bloom filter: ") + r.what());
            }

            if (filters.empty()) 
            {
                filters_ = std::vector<std::vector<uint64_t>>(hashCount_,
                                std::vector<uint64_t>(hashSize_ / 64, 0));
            }

            while (!(hashSize_ & (1 << bitsPerHash_)))
            {
                bitsPerHash_++;
            }
        }

        bool lookup(const std::vector<uint64_t> &hashes) const
        {
            uint32_t shift = startFrom_;
            for (size_t i = 0; i < hashCount_; ++i, shift += bitsPerHash_)
            {
                uint64_t h = 0;
                if (shift / 64 == (shift + bitsPerHash_) / 64)
                {
                    h = ((hashes[shift / 64] >> (shift & 63)) & (hashSize_ - 1));
                }
                else
                {
                    h = (((hashes[shift / 64] >> (shift & 63)) |
                             (hashes[(shift / 64) + 1] << (64 - (shift & 63)))) &
                         (hashSize_ - 1));
                }
                auto blockIdx = (h >> 6);
                auto bit = h & 63;
                if (!(filters_[i][blockIdx] & (1 << bit)))
                {
                    return false;
                }
            }
            return true;
        }

        void add(const std::vector<uint64_t> &hashes)
        {
            uint32_t shift = startFrom_;
            for (size_t i = 0; i < hashCount_; ++i, shift += bitsPerHash_)
            {
                uint64_t h = 0;
                if (shift / 64 == (shift + bitsPerHash_) / 64)
                {
                    h = (hashes[shift / 64] >> (shift % 64)) & (hashSize_ - 1);
                }
                else
                {
                    h = ((hashes[shift / 64] >> (shift % 64)) | (hashes[shift / 64 + 1] << (64 - shift % 64))) &
                        (hashSize_ - 1);
                }

                auto hashIdx = h / 64;
                auto bit = h & 63;
                filters_[i][hashIdx] = filters_[i][hashIdx] | (1 << bit);
            }
        }


    private:
        uint32_t hashSize_;
        uint32_t bitsPerHash_ = 0;
        uint32_t hashCount_;
        uint32_t startFrom_;
        std::vector<std::vector<uint64_t>> filters_;
    };

} //namespace hobf

