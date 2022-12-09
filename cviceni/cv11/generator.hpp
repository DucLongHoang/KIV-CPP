#pragma once

class Generator
{
	private:
		size_t mNumber_Count = 0;

		double mMean = 0;
		double mSigma = 0;

		double mMin = 0.0;
		double mMax = 0.0;

	public:
		Generator() = default;
		virtual ~Generator() = default;

		void Set_Number_Count(size_t count)
		{
			mNumber_Count = count;
		}
		void Set_Mean(double mean)
		{
			mMean = mean;
		}
		void Set_Sigma(double sigma)
		{
			mSigma = sigma;
		}
		void Set_Limits(double minval, double maxval)
		{
			mMin = minval;
			mMax = maxval;
		}

		void Generate();
};
