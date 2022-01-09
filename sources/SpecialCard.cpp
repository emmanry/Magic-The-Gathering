#include <iostream>
#include <string>

#include "../includes/SpecialCard.hpp"
#include "../includes/Card.hpp"

SpecialCard::SpecialCard(std::string name, int token, int id, std::string info): 
Card(name, token), m_id(id), m_info(info){
    std::cout << "[SpecialCard] : Création de " << this << std::endl;
}

SpecialCard::~SpecialCard(){
    std::cout << "[SpecialCard] : Destruction de " << this << std::endl;
}

int SpecialCard::get_id() const{
    return m_id;
}

std::string SpecialCard::get_info() const{
    return m_info;
}

void SpecialCard::set_id(int id){
    m_id = id;
}

void SpecialCard::set_info(std::string info){
    m_info = m_info;
}