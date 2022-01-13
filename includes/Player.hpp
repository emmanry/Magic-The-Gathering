#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <string>

#include "Game.hpp"
#include "Card.hpp"
#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Battlefield.hpp"

class Player {
    private:
        Game const&           m_game;
        std::string           m_name;
        int                   m_hp;
        bool                  m_played_land;
        Battlefield           m_battlefield;
        std::vector<Card*>    m_deck;
        std::vector<Card*>    m_graveyard;
        std::vector<Card*>    m_library;
        std::vector<Card*>    m_hand;

    public:
        Player(Game const& g, std::string s);
        ~Player();

        Game                  get_game() const;
        std::string           get_name() const;
        int                   get_hp() const;
        bool                  get_played_land() const;
        Battlefield           get_battlefield() const;
        std::vector<Card*>    get_deck() const;
        std::vector<Card*>    get_graveyard() const;
        std::vector<Card*>    get_library() const;
        std::vector<Card*>    get_hand() const;
  
        void                  set_name(std::string s);
        void                  set_hp(int i);
        void                  set_played_land(bool b);
  
        void                  draw_card();
        void                  discard_card(Card* c);
        void                  shuffle_library();
        void                  play_card(Card* c);
        void                  engage_card(BasicCard* bc);
        void                  disengage_card(BasicCard* bc);
        std::vector<Creature> attack();
        void                  choose_defenders(std::vector<Creature> opponents, Player other_player);
        void                  deflect_attack(Creature opponent, std::vector<Creature> defenders, Player other_player);
        void                  battle_creature(Creature opponent, Creature defender, Player other_player);
        void                  destroy_card(Card* c);
        void                  play_ritual(Ritual c);
        void                  loose();
  
        void                  print() const;

};

#endif
