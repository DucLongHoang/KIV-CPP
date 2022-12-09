#pragma once

#include <vector>
#include <string>

class IAnalyzer
{
	public:
		~IAnalyzer() = default;

		virtual bool Load() = 0;

		virtual bool Analyze() = 0;

		virtual bool Save_Image(const std::string& out) = 0;

		virtual size_t Get_File_Size() const = 0;
};

class DummyAnalyzer : public IAnalyzer
{
	public:
		DummyAnalyzer() = default;
		virtual ~DummyAnalyzer() = default;

		virtual bool Load() override { return true; }

		virtual bool Analyze() override { return true; }

		virtual bool Save_Image(const std::string& out) override { return true; }

		virtual size_t Get_File_Size() const override { return 0; }
};

class Dist2DAnalyzer : public IAnalyzer
{
	private:
		std::vector<std::vector<size_t>> Histogram;

		double* mData = nullptr;

		size_t mFile_Size = 0;

		size_t mMaximum = 0;

	public:
		Dist2DAnalyzer() = default;
		virtual ~Dist2DAnalyzer()
		{
			if (mData)
				delete[] mData;
		}

		virtual bool Load() override;

		virtual bool Analyze() override;

		virtual bool Save_Image(const std::string& out) override;

		virtual size_t Get_File_Size() const override { return mFile_Size; }
};