#include <iostream>
#include <string>

#include "Card.hpp"
#include "SpecialCard.hpp"
#include "Enchantment.hpp"

Enchantment::Enchantment(std::string name, int token, int id, std::string info): 
SpecialCard(name, token, id, info){}

Enchantment::~Enchantment(){}