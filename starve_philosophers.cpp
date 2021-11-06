#include <thread>
#include <iostream>
#include <random>
#include <chrono>
#include <mutex>

#define COUNT 5
#define THINKING 1
#define HUNGRY 0
#define EATING 2
#define LEFT(i)((i - 1 + COUNT) % 5)
#define RIGHT(i)((i + 1) % 5)

using namespace std;

const string NAMES[] = {"Socrates","Plato","Aristotle","Democritus","Xenophanes"};
static int PHILOSOPHERS_STATUS[] = {1,1,1,1,1};
mutex mtx;

int THREAD_LOCAL_RANDOM_INT(int min, int max){
	thread_local random_device rd;
	thread_local mt19937 rng(rd());
	uniform_int_distribution<int> uni(min,max);
	return uni(rng);
}

class PHILOSOPHER{
private:
	int ID;
	string NAME;
	int STATUS;
public:
	void SET_PHILOSOPHER_ALL(string NAME, int ID, int STATUS){
		this->NAME = NAME;
		this->ID = ID;
		this->STATUS = STATUS;
	};
	string GET_NAME(){
		return NAME;
	};
	int GET_STATUS(){
		return STATUS;
	};
	int GET_ID(){
		return ID;
	};
	void SET_STATUS(int STATUS){
		this->STATUS = STATUS;
	};
};

void PHILOSOPHER_LIVE(PHILOSOPHER philosopher) {
	cout << "Philosopher " << philosopher.GET_NAME() << " is now avaliable!" <<  endl;
	this_thread::sleep_for(chrono::milliseconds(2000ms));
		while(1 == 1){
		switch (philosopher.GET_STATUS()) {
			case 1:
				cout << "Philosopher " << philosopher.GET_NAME() << " thinking...." <<  endl;
  				this_thread::sleep_for(chrono::milliseconds(THREAD_LOCAL_RANDOM_INT(2000,3000)));
				PHILOSOPHERS_STATUS[philosopher.GET_ID()] = HUNGRY;
				philosopher.SET_STATUS(HUNGRY);
				cout << philosopher.GET_NAME() << " hungry (place " << philosopher.GET_ID() << ")" << endl;
  				break;
			case 0:
				mtx.lock();
					if((PHILOSOPHERS_STATUS[LEFT(philosopher.GET_ID())] != EATING)&&(PHILOSOPHERS_STATUS[RIGHT(philosopher.GET_ID())] != EATING)){
						PHILOSOPHERS_STATUS[philosopher.GET_ID()] = EATING;
						philosopher.SET_STATUS(EATING);
		                                cout << " Philosopher " << philosopher.GET_NAME() << " eating , his place at the table " << philosopher.GET_ID() + 1 <<  endl;
						cout << LEFT(philosopher.GET_ID()) + 1 << " and " << RIGHT(philosopher.GET_ID()) + 1 << " place locked!" << endl;
					}
				mtx.unlock();
  				break;
			case 2:
                                this_thread::sleep_for(chrono::milliseconds(THREAD_LOCAL_RANDOM_INT(2000,3000)));
                                cout << "  Philosopher " << philosopher.GET_NAME() << " finish eating!" <<  endl;
				cout << LEFT(philosopher.GET_ID()) + 1 << " and " << RIGHT(philosopher.GET_ID()) + 1 << " place unlocked!" << endl;
                                PHILOSOPHERS_STATUS[philosopher.GET_ID()] = THINKING;
				philosopher.SET_STATUS(THINKING);
				break;

		default:
  			cout << "EL PROBLEMO" << endl;
  			break;
		}
	}
}

int main() {
	PHILOSOPHER PHILOSOPHERS[COUNT];
	for(int id = 0; id < COUNT; id++){
		PHILOSOPHERS[id].SET_PHILOSOPHER_ALL(NAMES[id],id,THINKING);
	}
	thread PHILOSOPHER_1(PHILOSOPHER_LIVE,PHILOSOPHERS[0]);
	thread PHILOSOPHER_2(PHILOSOPHER_LIVE,PHILOSOPHERS[1]);
	thread PHILOSOPHER_3(PHILOSOPHER_LIVE,PHILOSOPHERS[2]);
	thread PHILOSOPHER_4(PHILOSOPHER_LIVE,PHILOSOPHERS[3]);
	thread PHILOSOPHER_5(PHILOSOPHER_LIVE,PHILOSOPHERS[4]);
	PHILOSOPHER_1.join();
	PHILOSOPHER_2.join();
	PHILOSOPHER_3.join();
	PHILOSOPHER_4.join();
	PHILOSOPHER_5.join();
}
 
