#ifndef BATTLEFIELD_HPP
#define BATTLEFIELD_HPP

#include <vector>

#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Enchantment.hpp"

class Battlefield {

	private:
		std::vector<BasicCard*>		m_basic_cards;
		std::vector<Enchantment*>	m_enchantments;

	public:
		Battlefield();
		Battlefield(const std::vector<Card*>& basic_cards, const std::vector<Enchantment*>& enchantments);
		~Battlefield();

		std::vector<BasicCard*>		get_basic_cards() const;
		std::vector<Enchantment*>	get_enchantments() const;

		void						set_basic_cards(std::vector<BasicCard*> bc);
		void						set_enchantments(std::vector<Enchantment*> e);

		void						sort_basic_cards();

		void						remove_basic_card(BasicCard* bc);
		void						remove_enchantment(Enchantment* e);
		
		
		void						place_basic_card(BasicCard* bc);
		void 						place_enchantment(Enchantment* e);
		std::vector<Creature*>		get_engaged_creatures();
		std::vector<Creature*>		get_available_creatures();
		std::vector<Land*>			get_available_lands();

		void						disengage_card(BasicCard* bc);
		bool						is_playable(Card* c); 

        void                        engage_lands(Cost* c);

		void						print();
		std::string					to_string();

};

#endif
