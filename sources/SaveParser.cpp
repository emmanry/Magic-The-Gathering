#include <iostream>
#include <vector>
#include <algorithm>

#include "SaveParser.hpp"
#include "CardParser.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Card.hpp"
#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Ritual.hpp"
#include "Enchantment.hpp"
#include "Cost.hpp"
#include "Battlefield.hpp"
#include "FonctionsAux.hpp"

/*
    Tokens utilisés pour parser une save
*/
const std::string SaveParser::begin_game = "#BEGIN_GAME";
const std::string SaveParser::end_game = "#END_GAME";
const std::string SaveParser::player_turn = "#PLAYER_TURN: ";
const std::string SaveParser::phase = "#PHASE: ";
const std::string SaveParser::begin_player = "#BEGIN_PLAYER";
const std::string SaveParser::end_player = "#END_PLAYER";
const std::string SaveParser::played_land = "#PLAYED_LAND: ";
const std::string SaveParser::name = "#NAME: ";
const std::string SaveParser::hp = "#HP: ";
const std::string SaveParser::begin_library = "#BEGIN_LIBRARY";
const std::string SaveParser::end_library = "#END_LIBRARY";
const std::string SaveParser::begin_graveyard = "#BEGIN_GRAVEYARD";
const std::string SaveParser::end_graveyard = "#END_GRAVEYARD";
const std::string SaveParser::begin_hand = "#BEGIN_HAND";
const std::string SaveParser::end_hand = "#END_HAND";
const std::string SaveParser::begin_battlefield = "#BEGIN_BATTLEFIELD";
const std::string SaveParser::end_battlefield = "#END_BATTLEFIELD";
const std::string SaveParser::begin_enchantments = "#BEGIN_ENCHANTMENTS";
const std::string SaveParser::end_enchantments = "#END_ENCHANTMENTS";
const std::string SaveParser::begin_card_list = "#BEGIN_CARD_LIST";
const std::string SaveParser::end_card_list = "#END_CARD_LIST";
const std::string SaveParser::begin_card = "#BEGIN_CARD";
const std::string SaveParser::end_card = "#END_CARD";
const std::string SaveParser::classcard = "#CLASS: ";
const std::string SaveParser::token = "#TOKEN: ";
const std::string SaveParser::engaged = "#ENGAGED: ";
const std::string SaveParser::power_toughness = "#POWER_TOUGHNESS: ";
const std::string SaveParser::abilities = "#ABILITIES: ";
const std::string SaveParser::types = "#TYPES: ";
const std::string SaveParser::cost = "#COST: ";
const std::string SaveParser::first_turn = "#FIRST_TURN: ";
const std::string SaveParser::effects = "#EFFECTS: ";
const std::string SaveParser::value = "#VALUE: ";


/*
    Renvoie toutes les lignes contenues entre la première apparition du token begin et la première apparition du token end dans data.
*/
std::vector<std::string> SaveParser::crop(const std::string& begin, const std::string& end, std::vector<std::string>& data) {
    std::vector<std::string> ret = {};
    for(unsigned int i = 0; i < data.size(); i++) {
        if(data[i].find(begin) != std::string::npos) {
            for(unsigned int j = 1; ((j < data.size() - i)); j++) {
                if (data[i+j].find(end) != std::string::npos) {
                    ret = { data.begin()+i+1, data.begin()+i+j };
                    data.erase(data.begin()+i, data.begin()+i+j+1);
                    return ret;
                }
            }
            i--;
        }    
    }
    return ret;
}

/*
    Renvoie l'entier lu sur la chaîne de caractère contenant la clé key.
    -1 sinon.
 */
int SaveParser::extract_int(const std::string& key, const std::string& data) {
    auto s = extract_data(key, data);
    try {
        return stoi(s);
    } catch (std::invalid_argument& e) {
        return -1;
    }
}

/*
    Renvoie un tableau d'entiers sur une ligne contenant la clé key.
    [] sinon.
 */
std::vector<int> SaveParser::extract_int_list(const std::string& key, std::string& data) {
    std::string s = extract_data(key, data);
    return CardParser::parse_int_list(s);
}

/*
    Renvoie la valeur (string) après la clé si elle est présente.
 */
std::string SaveParser::extract_data(const std::string& key, const std::string& data) {
    return (data.find(key) != std::string::npos) ? data.substr(key.size()) : "";
}

/*
    Extraie la ligne de data contenant la clé key.
    "" si elle n'est pas présente.
 */
std::string SaveParser::extract_line(const std::string& key, std::vector<std::string>& data) {
    size_t i = 0;
    bool end = false;

    for(auto& s : data) {
        if (s.find(key) != std::string::npos) {
            end = true;
            break;
        }
        i++;
    }

    if (end) {
        auto res = data[i];
        data.erase(data.begin() + i);
        return res;
    } else {
        return "";
    }
}

/*
    Parse et renvoie le player lu dans data.
*/
Player* SaveParser::extract_player(std::vector<std::string>& data) {
    if (data.size() == 0)
        throw std::invalid_argument("Lecture de Player");

    std::string name = extract_data(SaveParser::name, extract_line(SaveParser::name, data));
    int hp = extract_int(SaveParser::hp, extract_line(SaveParser::hp, data));
    if (hp < 1)
        throw std::invalid_argument("Lecture de HP");

    int played_land = extract_int(SaveParser::played_land, extract_line(SaveParser::played_land, data));
    if (played_land < -1)
        throw std::invalid_argument("Erreur dans la lecture de played_land");

    auto cropped_data = crop(SaveParser::begin_battlefield, SaveParser::end_battlefield, data);
    Battlefield* b = extract_battlefield(cropped_data);
    cropped_data = crop(SaveParser::begin_hand, SaveParser::end_hand, data);
    std::vector<Card*> hand = extract_cards(cropped_data);
    cropped_data = crop(SaveParser::begin_library, SaveParser::end_library, data);
    std::vector<Card*> library = extract_cards(cropped_data);
    cropped_data = crop(SaveParser::begin_graveyard, SaveParser::end_graveyard, data);
    std::vector<Card*> graveyard = extract_cards(cropped_data);

    Player* p = new Player(name);
    p->set_hp(hp);
    p->set_played_land(played_land);
    p->set_hand(hand);
    p->set_library(library);
    p->set_graveyard(graveyard);
    p->set_battlefield(b);
    return p;
}


/*
    Parse et renvoie la carte lue dans data.
*/
Card* SaveParser::extract_card(std::vector<std::string>& data) {
    if (data.size() == 0) {
        return nullptr;
    }
        
    Card* card = nullptr;

    switch (extract_int(SaveParser::classcard, extract_line(SaveParser::classcard, data))) {
        case Card_class::CREATURE:
            card = SaveParser::extract_creature(data);
            break;
        case Card_class::LAND:
            card = SaveParser::extract_land(data);
            break;
        case Card_class::RITUAL:
            card = SaveParser::extract_ritual(data);
            break;
        case Card_class::ENCHANTEMENT:
            card = SaveParser::extract_enchantment(data);
            break;
        default:
            break;
    }
    if (card == nullptr)
        throw std::invalid_argument("Extraction d'un carte a échoué.");
    return card;
}


/*
    Parse et renvoie la créature lue dans data.
*/
Creature* SaveParser::extract_creature(std::vector<std::string>& data) {
    int token = extract_int(SaveParser::token, extract_line(SaveParser::token, data));
    if (token > Token::Count || token < 0)
        throw std::invalid_argument("Token de Creature invalide.");

    std::string name = extract_data(SaveParser::name, extract_line(SaveParser::name, data));
    if (name == "")
        throw std::invalid_argument("Name de Creature non renseigné.");

    int engaged = extract_int(SaveParser::engaged, extract_line(SaveParser::engaged, data));
    if ((engaged & 1) != engaged)
        throw std::invalid_argument("Engaged de Creature invalide.");

    int first_turn = extract_int(SaveParser::first_turn, extract_line(SaveParser::first_turn, data));
    if ((first_turn & 1) != first_turn)
        throw std::invalid_argument("First_turn de Creature invalide.");

    std::string cropped_line = extract_line(SaveParser::power_toughness, data);
    std::vector<int> power_toughness = extract_int_list(SaveParser::power_toughness, cropped_line);
    if (power_toughness.size() != 4)
        throw std::invalid_argument("Power/toughness de Creature invalide.");

    cropped_line = extract_line(SaveParser::abilities, data);
    std::vector<int> abilities = extract_int_list(SaveParser::abilities, cropped_line);
    cropped_line = extract_line(SaveParser::types, data);
    std::vector<int> types = extract_int_list(SaveParser::types, cropped_line);
    cropped_line = extract_line(SaveParser::cost, data);
    std::vector<int> cost = extract_int_list(SaveParser::cost, cropped_line);
    if (cost.size() != 6)
        throw std::invalid_argument("Cost de Creature invalide.");

    auto cropped_data = crop(SaveParser::begin_enchantments, SaveParser::end_enchantments, data);
    std::vector<Card*> ench = extract_cards(cropped_data);
    std::vector<Enchantment*> enchantments = {};
    for(auto e : ench) {
        if (e->get_class() == Card_class::ENCHANTEMENT) {
            enchantments.push_back(dynamic_cast<Enchantment*>(e));
        } else {
            for(auto c : ench)
                delete c;
            for(auto c : enchantments)
                delete c;
            throw std::invalid_argument("Carte autre d'enchantements dans les enchantements de Créature.");
        }
    }

    Creature* c =  new Creature(Card_class::CREATURE, name, token, power_toughness[0], power_toughness[2], abilities, types, new Cost(cost[0], cost[1], cost[2], cost[3], cost[4], cost[5]));        

    c->set_is_first_turn(first_turn);
    c->set_power_current(power_toughness[2]);
    c->set_toughness_current(power_toughness[3]);
    c->set_engaged(engaged);

    return c;    
}

/*
    Parse et renvoie le terrain lu dans data.
*/
Land* SaveParser::extract_land(std::vector<std::string>& data) {
    int token = extract_int(SaveParser::token, extract_line(SaveParser::token, data));
    if (token > Token::Count || token < 0)
        throw std::invalid_argument("Token de Land invalide.");

    std::string name = extract_data(SaveParser::name, extract_line(SaveParser::name, data));
    if (name == "")
        throw std::invalid_argument("Name de Land non renseigné.");

    int engaged = extract_int(SaveParser::engaged, extract_line(SaveParser::engaged, data));
    if ((engaged & 1) != engaged)
        throw std::invalid_argument("Engaged de Land invalide.");

    int value = extract_int(SaveParser::value, extract_line(SaveParser::value, data));
    if (value < 0)
        throw std::invalid_argument("Value de Land invalide.");

    auto cropped_data = crop(SaveParser::begin_enchantments, SaveParser::end_enchantments, data);
    std::vector<Card*> ench = extract_cards(cropped_data);
    std::vector<Enchantment*> enchantments = {};
    for(auto e : ench) {
        if (e->get_class() == Card_class::ENCHANTEMENT) {
            enchantments.push_back(dynamic_cast<Enchantment*>(e));
        } else {
            for(auto c : ench)
                delete c;
            for(auto c : enchantments)
                delete c;
            throw std::invalid_argument("Carte autre d'enchantements dans les enchantements de Land.");
        }
    }

    Land* l = new Land(Card_class::LAND, name, token, value);
    for(auto e : enchantments) {
        l->add_enchantment(e);
    }
    l->set_engaged(engaged);

    return l;
}

/*
    Parse et renvoie le rituel lu dans data.
*/
Ritual* SaveParser::extract_ritual(std::vector<std::string>& data) {
    int token = extract_int(SaveParser::token, extract_line(SaveParser::token, data));
    if (token > Token::Count || token < 0)
        throw std::invalid_argument("Token de Ritual invalide.");

    std::string name = extract_data(SaveParser::name, extract_line(SaveParser::name, data));
    if (name == "")
        throw std::invalid_argument("Name de Ritual non renseigné.");
    std::string cropped_line = extract_line(SaveParser::effects, data);
    std::vector<int> effects = extract_int_list(SaveParser::effects, cropped_line);

    cropped_line = extract_line(SaveParser::cost, data);
    std::vector<int> cost = extract_int_list(SaveParser::cost, cropped_line);
    if (cost.size() != 6)
        throw std::invalid_argument("Cost de Ritual invalide.");

    Ritual* r = new Ritual(Card_class::RITUAL, name, token, new Cost(cost[0], cost[1], cost[2], cost[3], cost[4], cost[5]), effects);

    return r;
}

/*
    Parse et renvoie l'enchantement lu dans data.
*/
Enchantment* SaveParser::extract_enchantment(std::vector<std::string>& data) {
    int token = extract_int(SaveParser::token, extract_line(SaveParser::token, data));
    if (token > Token::Count || token < 0)
        throw std::invalid_argument("Token d'Enchantment invalide.");

    std::string name = extract_data(SaveParser::name, extract_line(SaveParser::name, data));
    if (name == "")
        throw std::invalid_argument("Name d'Enchantment non renseigné.");

    std::string cropped_line = extract_line(SaveParser::effects, data);
    std::vector<int> effects = extract_int_list(SaveParser::effects, cropped_line);
    cropped_line = extract_line(SaveParser::cost, data);
    std::vector<int> cost = extract_int_list(SaveParser::cost, cropped_line);
    if (cost.size() != 6)
        throw std::invalid_argument("Cost d'Enchantment invalide.");

    Enchantment* e = new Enchantment(Card_class::ENCHANTEMENT, name, token, new Cost(cost[0], cost[1], cost[2], cost[3], cost[4], cost[5]), effects);
    return e;
}

/*
    Parse et renvoie une liste de cartes lue dans data.
*/
std::vector<Card*> SaveParser::extract_cards(std::vector<std::string>& data) {
    std::vector<Card*> v{};
    
    std::vector<std::string> cropped_data;
    size_t length;
    
    do {
        cropped_data = crop(SaveParser::begin_card, SaveParser::end_card, data);
        length = cropped_data.size();
    
        auto res = extract_card(cropped_data);
    
        if (res != nullptr)
            v.push_back(res);
    } while (length > 0);
    return v;
}

/*
    Parse et renvoie le champ de bataille lu dans data.
*/
Battlefield* SaveParser::extract_battlefield(std::vector<std::string>& data) {
    auto cropped_data = crop(SaveParser::begin_card_list, SaveParser::end_card_list, data);
    std::vector<Card*> cards = extract_cards(cropped_data);

    cropped_data = crop(SaveParser::begin_enchantments, SaveParser::end_enchantments, data);
    std::vector<Card*> ench = extract_cards(cropped_data);
    std::vector<Enchantment*> enchantments = {};
    for(auto e : ench) {
        if (e->get_class() == Card_class::ENCHANTEMENT) {
            enchantments.push_back(dynamic_cast<Enchantment*>(e));
        } else {
            for(auto c : ench)
                delete c;
            for(auto c : cards)
                delete c;
            for(auto c : enchantments)
                delete c;
            throw std::invalid_argument("Carte autre d'enchantements dans les enchantements de Battlefield.");
        }
    }

    for(auto c : enchantments)
        if (c->get_class() != Card_class::ENCHANTEMENT)
            throw std::invalid_argument("Un non enchantement reconnu dans Battlefield::m_enchantments.");

    for(auto c : cards)
        if (c->get_class() != Card_class::CREATURE && c->get_class() != Card_class::LAND)
            throw std::invalid_argument("Un carte autre que BasicCard reconnu dans Battlefield::m_basic_cards.");

    return new Battlefield(cards, enchantments);
}

/*
    Parse et renvoie la partie lu dans data.
*/
Game* SaveParser::extract_game(std::vector<std::string>& data) {
    if (data.size() == 0)
        throw std::invalid_argument("Lecture de Game vide.");

    int player_turn = extract_int(SaveParser::player_turn, extract_line(SaveParser::player_turn, data));
    if ((player_turn & 1) != player_turn)
        throw std::invalid_argument("Player_turn invalide.");

    int phase = extract_int(SaveParser::phase, extract_line(SaveParser::phase, data));
    if (phase < 0 || phase > 2)
        throw std::invalid_argument("Phase invalide.");

    auto cropped_data = crop(SaveParser::begin_player, SaveParser::end_player, data);
    Player* p1 = extract_player(cropped_data);
    if (p1 == nullptr)
        throw std::invalid_argument("Extraction d'un Player.");

    cropped_data = crop(SaveParser::begin_player, SaveParser::end_player, data);
    Player* p2 = extract_player(cropped_data);
    if (p2 == nullptr)
        throw std::invalid_argument("Extraction d'un Player.");

    Game* g = new Game();

    g->set_player_turn(player_turn);
    g->set_phase(phase);

    p1->set_opponent(p2);
    p2->set_opponent(p1);

    g->set_players(p1, p2);

    return g;
}


/*
    Parse et renvoie la partie lu dans s.
*/
Game* SaveParser::load(std::string& s) {
    auto data = CardParser::get_line_data(s);
    CardParser::clean_data(data);
    
    for(auto& i : data)
    	if (!i.empty() && (i.back() == '\r' || i.back() == '\n' || i.back() == '\t'))
    		i = i.substr(0, i.size()-1);
	
    std::erase_if(data, [] (const std::string& s) { return s == "" || s.find('\r') == 0; });
	std::erase_if(data, [] (const std::string& s) { return s.find(CardParser::comment) == 0; });

    auto cropped_data = crop(SaveParser::begin_game, SaveParser::end_game, data);

    return extract_game(cropped_data);    
}