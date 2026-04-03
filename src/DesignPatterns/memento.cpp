#include "DesignPatterns/memento.hpp"
#include <cstddef>
#include <iostream>
#include <utility>

Memento::Snapshot Memento::save()
{
	// create a snapshot instance
	Memento::Snapshot snapshot;

	// save to the snapshot
	_saveToSnapshot(snapshot);

	// give the instance
	return snapshot;
}

void Memento::load(const Memento::Snapshot& snapshot)
{
	// load the data from the snapshot
	_loadFromSnapshot(snapshot);
}


SaveableObject::SaveableObject(): name("CHEESE"), counter(10), age(21), progress("NO PROGRESS") {}



void SaveableObject::_saveToSnapshot(Memento::Snapshot& snapshot) const
{
	snapshot.saveData(
		MAKE_SNAPSHOT_DATA_ENTRY(name),
		MAKE_SNAPSHOT_DATA_ENTRY(counter),
		MAKE_SNAPSHOT_DATA_ENTRY(age),
		MAKE_SNAPSHOT_DATA_ENTRY(progress)
	);
}

void SaveableObject::_loadFromSnapshot(const Memento::Snapshot& snapshot)
{
	SET_DATA_ENTRY(name, snapshot);
	SET_DATA_ENTRY(counter, snapshot);
	SET_DATA_ENTRY(age, snapshot);
	SET_DATA_ENTRY(progress, snapshot);
}

void SaveableObject::print()
{
	std::cout << "Name: " << name << " Counter: " << counter << " Age: " << age << " Progress: " << progress << std::endl;
}

void SaveableObject::setName(const std::string& new_name) { name = new_name; }
void SaveableObject::setAge(size_t new_age) { age = new_age; }