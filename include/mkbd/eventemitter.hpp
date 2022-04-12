#ifndef __MKBD_EVENT_SYSTEM_HPP__
#define __MKBD_EVENT_SYSTEM_HPP__

#include <vector>
#include <unordered_map>
#include <functional>
#include <cstddef>

#define SET_FID static std::size_t FID[0]

class EventEmitter {

	struct GroupId {
		std::uintptr_t mValue = 0;

		GroupId(std::uintptr_t value) { mValue = value; };

		template<class T>
		GroupId(T* ptr) { mValue = (std::uintptr_t)ptr; };

		bool operator==(const GroupId& other) {
			return mValue == other.mValue;
		};
	};

	struct Event {
		void* vFunc;
		GroupId group = 0;
		bool once = false;

		Event(void* vFunc, GroupId group)
		: vFunc(vFunc), group(group) {};

		template<class... Args, class T = std::function<void(Args...)>>
		void invoke(Args&&... args) {
			T& func = *reinterpret_cast<T*>(vFunc);
			func(std::forward<Args>(args)...);
		};
	};

	using EventList = std::vector<Event>;
	using EventMap = std::unordered_map<std::string, EventList>;

	EventMap events;

public:
	EventEmitter(void) {};

	template<class... Args, class T = std::function<void(Args...)>>
	void on(std::string name, T&& t, GroupId group = -1) {
		events[name].push_back(Event(new T(std::forward<T>(t)), group));
	};

	template<class... Args, class T = std::function<void(Args...)>>
	void once(std::string name, T&& t, GroupId group = -1) {
		Event e(new T(std::forward<T>(t)), group);
		e.once = true;
		events[name].push_back(e);
	};

	template<class... Args, class T = std::function<void(Args...)>>
	void emit(std::string name, Args... args) {
		if (events.find(name) == events.end())
			return;

		EventList& list = events[name];

		std::vector<EventList::iterator> toDelete;

		for (auto it = list.begin(); it != list.end(); it++) {
			it->invoke(std::forward<Args>(args)...);

			if (it->once)
				toDelete.push_back(it);
		}

		for (auto it : toDelete)
			list.erase(it);
	};

	void clear(std::string name) {
		if (events.find(name) != events.end())
			events[name].clear();
	};

	void clearGroup(GroupId group) {
		for (auto& el : events) {
			EventList& list = el.second;
			std::vector<EventList::iterator> toDelete;
			for (auto it = list.begin(); it != list.end(); it++) {
				if (it->group == group)
					toDelete.push_back(it);
			}

			for (auto it : toDelete)
				list.erase(it);
		}
	};
};

template<class... Args, class T>
auto asFunction(T&& f) {
	return std::function<void(Args...)>(f);
}

#endif