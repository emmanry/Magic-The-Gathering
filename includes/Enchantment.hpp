#ifndef ENCHANTMENT_HPP
#define ENCHANTMENT_HPP

#include "SpecialCard.hpp"

class Enchantment : public virtual SpecialCard{

    private:

    public:
        Enchantment(std::string, int, int, std::string, Cost*);
        ~Enchantment();

        virtual void print();

};

#endif
