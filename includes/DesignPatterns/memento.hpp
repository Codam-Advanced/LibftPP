#pragma once

#include <any>
#include <string>
#include <unordered_map>

#define MAKE_SNAPSHOT_DATA_ENTRY(member_variable) std::make_pair(#member_variable, member_variable)
#define SET_DATA_ENTRY(member_variable, snapshot) member_variable = snapshot.fetchData<typeof(member_variable)>(#member_variable)

class Memento
{
	public:
	
		class Snapshot 
		{

			public:

				Snapshot() = default;
				virtual ~Snapshot() = default;

				template<typename ...TArgs>
				void saveData(TArgs&&... p_variables)
				{
					// save the variable by key (name) and value of the variable
    				((_storage[p_variables.first] = std::forward<TArgs>(p_variables).second), ...);
				}

				template<typename T>
				T fetchData(std::string variable_name) const
				{
					// cast the std::any into the actual type and return the stored value
					return std::any_cast<T>(_storage.at(variable_name));
				}

			private:
				std::unordered_map<std::string, std::any> _storage;
		};

		Snapshot save();
		void load(const Memento::Snapshot& snapshot);

	private:
		virtual void _saveToSnapshot(Memento::Snapshot& snapshot) const = 0;
		virtual void _loadFromSnapshot(const Memento::Snapshot& snapshot) = 0;
};


class SaveableObject: public Memento
{
	// friend the class to allow access to the 
	// _saveToSnapshot & _loadFromSnapshot which will tell Memento how to save each object
	friend class Memento;

	public:
		SaveableObject();

		void print();
		void setName(const std::string& new_name);
		void setAge(size_t new_age);


	private:
		void _saveToSnapshot(Memento::Snapshot& snapshot) const override;
		void _loadFromSnapshot(const Memento::Snapshot& snapshot) override;

		// some data
		std::string 	name;
		size_t 			counter;
		size_t			age;
		std::string		progress;
};