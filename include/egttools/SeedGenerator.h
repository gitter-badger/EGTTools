//
// Created by Elias Fernandez on 17/05/2018.
//

#ifndef EGTTOOLS_SEEDGENERATOR_H
#define EGTTOOLS_SEEDGENERATOR_H

#include <random>
#include <thread>

namespace egttools { namespace Random {

    /**
    * This template function can be used to initialize a random generator
    * from a series of seeds.
    *
    * T his function will be active if N == 1
    *
    * @tparam T : type of random generator
    * @tparam N : number of randomly generated seeds to use
    * @return random generator of class T
    */
    template<class T = std::mt19937, std::size_t N = T::state_size * sizeof(typename T::result_type)>
    auto SeededRandomEngine() -> typename std::enable_if<N == 1, T>::type {
        std::random_device source;
        return T(source());
    }

    /**
    * This template function can be used to initialize a random generator
    * from a series of seeds.
    *
    * This function will be active if N > 1
    *
    * @tparam T : type of random generator
    * @tparam N : number of randomly generated seeds to use
    * @return random generator of class T
    */
    template<class T = std::mt19937, std::size_t N = T::state_size * sizeof(typename T::result_type)>
    auto SeededRandomEngine() -> typename std::enable_if<N >= 2, T>::type {
        std::random_device source;
        std::random_device::result_type random_data[(N - 1) / sizeof(source()) + 1];
        std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
        std::seed_seq seeds(std::begin(random_data), std::end(random_data));

        return T(seeds);
    }

    class SeedGenerator {
    public:
        /**
        * @brief This functions provices a pointer to a Seeder class
        * @return SeedGenerator
        */
        static SeedGenerator &getInstance();
        ~SeedGenerator() = default;

        /**
        * @brief This function generates a random number to seed other generators
        *
        * You can use this function to generate a random seed to seed other random generators
        * in you project. This avoids concurrency problems when doing parallel execution.
        *
        * @return A random unsigned long number
        */
        unsigned long int getSeed();

        /**
        * @brief This function sets the seed for the seed generator
        *
        * By default the generator is seeded either from a seed.in file or from random_device
        *
        * @param seed The seed for the random generator used to generate new seeds
        */
        void setMainSeed(unsigned long int seed);

        /**
        * @brief This function sets the seed for the seed generator
        *
        * By default the generator is seeded either from a seed.in file or from random_device
        *
        * @return main seed (unsigned long int)
        */
        [[nodiscard]] unsigned long int getMainSeed() const { return _rng_seed; }

    private:
        // Random generator
        std::mt19937_64 _rng_engine;

        // seed
        unsigned long int _rng_seed = 0;

        // Private constructor to prevent instancing
        SeedGenerator();
        explicit SeedGenerator(unsigned long int seed);
    };

    std::mt19937_64 * thread_local_generator();

} }// namespace egttools::Random

#endif//EGTTOOLS_RANDGEN_H
