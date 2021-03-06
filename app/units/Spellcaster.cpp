#include "../Exceptions.h"
#include "../states/State.h"
#include "../damage/Damage.h"
#include "SpellCaster.h"

Spellcaster::Spellcaster(const std::string& name, int damageValue, int hitPoints, int hitPointsLimit) : Unit(name, damageValue, hitPoints, hitPointsLimit) {
    addUnitType(Unit::TYPE_SPELLCASTER);
}

Spellcaster::~Spellcaster() {}

std::map<std::string, Spell>& Spellcaster::getSpellBook() {
    return m_spellBook;
}

Spell& Spellcaster::getSpellFromBook(const std::string& spellName) {
    std::map<std::string, Spell>::iterator it = m_spellBook.find(spellName);

    if ( it == m_spellBook.end() ) {
        throw NoSuchSpellInBook();
    }

    return it->second;
}

void Spellcaster::heal(Unit& patient, const std::string& spellName) {
    if ( isDead() || patient.isUnitHasType(TYPE_BERSERKER)) {
        return;
    }
    
    Spell& spell = getSpellFromBook(spellName);
    if ( spell.getType() != Spell::TYPE_HEALING ) {
        throw HealingWithNoHealSpell();
    }

    patient.addHitPoints(spell.getValue());
}

void Spellcaster::cast(Unit& enemy, const std::string& spellName) {
    if ( isDead() ) {
        return;
    }

    Spell& spell = getSpellFromBook(spellName);
    if ( spell.getType() != Spell::TYPE_BATTLE ) {
        throw AttackWithNotBattleSpell();
    }

    Damage dmg = Damage(spell.getValue(), Damage::TYPE_MAGIC);
    getState()->attack(enemy, dmg);
}

void Spellcaster::addSpellToBook(Spell& spell) {
    m_spellBook.insert(std::pair<std::string, Spell>(spell.getName(), spell));
}

void Spellcaster::showSpellBook() const {
    std::cout << "Spell book contains spells:" << std::endl;
    for ( std::map<std::string, Spell>::const_iterator it = m_spellBook.begin(); it != m_spellBook.end(); ++it ) {
        std::cout << it->second;
    }
    std::cout << std::endl;
}

