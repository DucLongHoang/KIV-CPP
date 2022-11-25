#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>
#include <array>
#include <chrono>

// Pro ted nereste, zda je v roure dost mista - to by nahravalo na implementaci semaforu a to ted nechceme (semafor - viz ZOS, PPR, UPP, PGS, ...)

class StupidPipe final
{
	public:
		StupidPipe()
		{
			//
		}

		void Write(char c)
		{
			// TODO: zamykat a notifikovat
            std::unique_lock<std::mutex> lock(mtx);

			mBuffer[mWrite_Cursor] = c;
			mWrite_Cursor = (mWrite_Cursor + 1) % mBuffer.size();

            // notify
            cv.notify_one();
		}

		char Read()
		{
			// TODO: zamykat a uspavat se, kdyz nic neni k dispozici
            // wait - condition - check if cursor moved
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() -> bool {
                return mWrite_Cursor > mRead_Cursor;
            });

			char c = mBuffer[mRead_Cursor];
			mRead_Cursor = (mRead_Cursor + 1) % mBuffer.size();
			return c;
		}

	private:
		std::array<char, 128> mBuffer;
		size_t mWrite_Cursor = 0;
		size_t mRead_Cursor = 0;
        std::mutex mtx;
        std::condition_variable cv;
};

// global mutex defined for synchronized print to console out
std::mutex printOutMutex;

void Producent_Thread(StupidPipe& r, char c, size_t delay)
{
	// TODO: synchronizujte vypisy tak, aby se v konzoli neprekryvaly a neprokladaly, tedy aby se nezamichaly do sebe
    // protect this with mutex
    {
        std::lock_guard<std::mutex> lck(printOutMutex);
	    std::cout << "Startuje producent '" << c << "'" << std::endl;
    }

	while (true)
	{
		r.Write(c);
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}
}

void Konzument_Thread(StupidPipe& r, size_t delay)
{
	// TODO: synchronizujte vypisy tak, aby se v konzoli neprekryvaly a neprokladaly, tedy aby se nezamichaly do sebe
    // protect this with mutex
    {
        std::lock_guard<std::mutex> lck(printOutMutex);
        std::cout << "Startuje konzument" << std::endl;
    }

	while (true)
	{
		char c = r.Read();

		std::cout << c;

		// TODO: tohle odstrante, jakmile budete mit implementovane zamykani
//		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}
}

int main(int argc, char** argv)
{
	StupidPipe roura;

	std::thread prod1(&Producent_Thread, std::ref(roura), 'a', 1500);
	std::thread prod2(&Producent_Thread, std::ref(roura), 'b', 1200);
	std::thread prod3(&Producent_Thread, std::ref(roura), 'c', 2300);

	std::thread konz(&Konzument_Thread, std::ref(roura), 300);

	prod1.join();
	prod2.join();
	prod3.join();
	konz.join();

	return 0;
}
