#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

//Template for creating csv file
ofstream fs;
string filename = "Schedule.csv";

//Declaring functions
void dayCalculation(int x, int y, int& z);

//Time structure
struct date
{
    int tm_sec;  //Time in seconds
    int tm_min;  //Time in minutes
    int tm_hour; //Time in hours
    int tm_mday; //Time in days
    int tm_mon;  //Time in months
    int tm_year; //Time in years
};

//Main function
int main()
{
    //Declaring variables
    tm date;           //Structure object that will be used in date calculation
    double days;       //Placeholder variable for days
    double rewardrate; //Reward rate in percentage 
    double actual;     //Actual reward rate for a given period(calculation 365 or 366 days)
    double ongoing;    //Ongoing reward rate for a given period in days or month
    double eth;        //Staking ETH amount
    double stakingDur; //Staking duration in months
    int rewDay;        //Reward day, for example 15
    string reinvest;   //Used for checking if user wants to reinvest
    double tempTime;   //Placeholder time used for calculation period of staking
    int TempDay;       //Placeholder variable for days
    int count = 1;     //Line counter for output to csv
    double TotRew = 0; //Total ETH reward(calculation)

    //Declaring time using structure
    time_t ttime = time(0);
    tm* time = localtime(&ttime);

    //Creates and opens csv file
    fs.open("schedule.csv");
    fs << "Line#, Reward Date, Investmest Amount, Reward Amount, Total Reward Amount to Date, Staking Reward Rate" << endl;

    //Variables for if the user knows when and how staking rates will change
    double newRate;
    string rate;
    int year;
    int month;

    //Getting user input

    //Waiting for user input in years
    cout << "Please input the start year of staking: ";
    cin >> time->tm_year;

    //Checking if user input in years is valid
    while (time->tm_year / 1000 != 2)
    {
        cout << "Please enter valid year: ";
        cin >> time->tm_year;
    }

    //Waiting for user input in months
    cout << "Input the start month of staking: ";
    cin >> time->tm_mon;

    //checking if user input in months is valid
    while (time->tm_mon < 0 || time->tm_mon > 12)
    {
        cout << "Please enter valid month: ";
        cin >> time->tm_mon;
    }

    //Calculating actual amount of days to compare if user imput is valid
    dayCalculation(time->tm_year, time->tm_mon, TempDay);
    tempTime = TempDay;

    //Waiting for user input in days
    cout << "Input the start day of staking: ";
    cin >> time->tm_mday;

    //Checking if user input in days is valid
    while (time->tm_mday > tempTime || time->tm_mday < 1)
    {
        cout << "Please enter valid day: ";
        cin >> time->tm_mday;
    }

    //Waiting for user input of reward rate in percents
    cout << "Input reward rate in percents: ";
    cin >> rewardrate;

    //Checking if reward rate in percents is valid
    while (rewardrate > 100 || rewardrate <= 0)
    {
        cout << "Please enter valid reward rate in percents: ";
        cin >> rewardrate;
    }

    //Waiting for user input of staking duration in months
    cout << "Enter staking duration in months: ";
    cin >> stakingDur;

    //Checking if staking duration in months is valid
    while (stakingDur < 1)
    {
        cout << "Please enter valid staking duration in months: ";
        cin >> stakingDur;
    }

    //Waiting for user input of the amount of ETH invested
    cout << "Enter initial investment amount in ETH: ";
    cin >> eth;

    //Checking if amount of ETH input is valid
    while (eth <= 0)
    {
        cout << "Please enter valid initial investment amount in ETH: ";
        cin >> eth;
    }

    //Waiting for user input of reward payment day
    cout << "Enter reward payment day: ";
    cin >> rewDay;

    //Checking if reward payment day input is valid
    while (rewDay > tempTime || rewDay < 1)
    {
        cout << "Please enter valid reward payment day: ";
        cin >> rewDay;
    }

    //Asking the user if they would like to reinvest the reward
    cout << "Would you like to reinvest rewards once you receive them or not press yes/no ";
    cin >> reinvest;

    //Checking if user input is valid
    if (reinvest != "yes" && reinvest != "no")
    {
        while (reinvest != "yes" && reinvest != "no")
        {
            cout << "Please enter valid input (yes/no)";
            cin >> reinvest;
            if (reinvest == "yes" || reinvest == "no")
            {
                break;
            }
        }
    }

    //Asking the user if they when and how the staking rate will change
    cout << "Do you know will the staking rate change yes/no: ";
    cin >> rate;

    //Checking if user input is valid
    if (rate != "yes" && rate != "no")
    {
        while (rate != "yes" && rate != "no")
        {
            cout << "Please enter valid input (yes/no)";
            cin >> rate;
            if (rate == "yes" || rate == "no")
            {
                break;
            }
        }
    }

    //If the answer is yes the program asks the user when and how it will change
    if (rate == "yes")
    {
        cout << "Enter the year of the change: ";
        //Input in years
        cin >> year;

        //Checking if user input in years is valid
        while (year < time->tm_year)
        {
            cout << "Please enter valid year: ";
            cin >> year;
        }

        //Input in months
        cout << "Please enter the month of the change: ";
        cin >> month;

        //Checking if user input in months is valid
        while ((month < 0 || month > 12) || (time->tm_mon > month && time->tm_year == year))
        {
            cout << "Please enter valid month: ";
            cin >> month;
        }

        cout << "What will be the new rate of staking: ";
        //Input of new staking percentage
        cin >> newRate;

        //Checking whether user input is valid
        while (newRate > 100 || newRate <= 0)
        {
            cout << "Please enter valid reward rate in percents: ";
            cin >> newRate;
        }
    }

    //Calculating reward rate for one year
    if ((time->tm_year % 4 == 0 && time->tm_year % 100 != 0) || (time->tm_year % 400 == 0))
    {
        actual = rewardrate / 366;
    }
    else
    {
        actual = rewardrate / 365;
    }

    //cout << parameters

    //Calculating investment, if user chooses to reinvest
    if (reinvest == "yes")
    {
        //Declaring placeholder variables
        int Tday;
        double ethRew;

        //Checking if the start date of staking is less than reward date
        if (time->tm_mday < rewDay)
        {
            //Calculating the amount of days left until reward day
            Tday = rewDay - time->tm_mday;

            //Calculating reward rate with previously calculated amount of days
            ongoing = actual * Tday;

            //Calculating reward amount with previously calculated reward rate
            ethRew = eth * (ongoing / 100);

            //After reward is calculated it is then reinvested with all ETH
            eth = eth + ethRew;

            //Calculating total reward during staking period
            TotRew = TotRew + ethRew;

            //Outputting first line of data to generated csv file
            fs << fixed << setprecision(6) << count << ", " << time->tm_year << "-" << time->tm_mon << "-" << rewDay << ", " << eth << ", " << ethRew << ", " << TotRew << ", " << rewardrate << "%" << endl;

            //Incrementing months because reward for one is already calculated
            time->tm_mon++;

            //Looping through every month left until staking period ends
            for (int i = 1; i <= stakingDur; i++)
            {
                //Incrementing count that is outputted to csv file that show number of line
                count++;

                //Calling function to calculate amount of days during next month
                dayCalculation(time->tm_year, time->tm_mon, TempDay);

                //Calculating the amount of days until the next reward
                Tday = TempDay - rewDay;
                Tday = Tday + rewDay;

                //Checking if the month is greater than 12, if it is then it should be set to 1 and incrementing the year also
                if (time->tm_mon > 12)
                {
                    time->tm_mon = 1;
                    time->tm_year++;
                }

                //Calculating new yearly reward rate if it changes
                if (rate == "yes" && (time->tm_year == year && time->tm_mon == month + 1))
                {
                    actual = newRate / 365;
                    rewardrate = newRate;
                }

                //Calculating ongoing reward rate with previously calculated amount of days left
                ongoing = actual * Tday;

                //Calculating ethereum reward
                ethRew = eth * (ongoing / 100);

                //Reinvesting the reward
                eth = ethRew + eth;

                //Calculating total reward rate
                TotRew = TotRew + ethRew;

                //Checking if staking duration is ending if it is then reward day is re-adjusted
                if (i < stakingDur)
                {
                    fs << fixed << setprecision(6) << count << ", " << time->tm_year << "-" << time->tm_mon << "-" << rewDay << ", " << eth << ", " << ethRew << ", " << TotRew << ", " << rewardrate << "%" << endl;
                }
                else
                {
                    fs << fixed << setprecision(6) << count << ", " << time->tm_year << "-" << time->tm_mon << "-" << time->tm_mday << ", " << eth << ", " << ethRew << ", " << TotRew << ", " << rewardrate << "%" << endl;
                }
                //Incrementing month
                time->tm_mon++;
            }
        }
        //Checking if the start date of staking is greater than reward date
        else if (time->tm_mday > rewDay)
        {
            //Incrementing month, because reward will be given in the next month
            time->tm_mon++;

            //Checking if the month is greater than 12, if it is then it should be set to 1 and incrementing the year also
            if (time->tm_mon > 12)
            {
                time->tm_mon = 1;
                time->tm_year++;
            }

            //Calculating new yearly reward rate if it changes
            if (rate == "yes" && (time->tm_year == year && time->tm_mon == month + 1))
            {
                actual = newRate / 365;
                rewardrate = newRate;
            }

            //Calling function to calculate amount of days during next month
            dayCalculation(time->tm_year, time->tm_mon, TempDay);

            //Calculating the amount of days left until reward day
            Tday = TempDay - time->tm_mday + rewDay;

            //Calculating reward rate with previously calculated amount of days
            ongoing = actual * Tday;

            //Calculating ETH reward
            ethRew = eth * (ongoing / 100);

            //Reinvesting ETH
            eth = eth + ethRew;

            //Calculating total reward to date
            TotRew = TotRew + ethRew;

            //Outputting first line to generated csv file
            fs << fixed << setprecision(6) << count << ", " << time->tm_year << "-" << time->tm_mon << "-" << rewDay << ", " << eth << ", " << ethRew << ", " << TotRew << ", " << rewardrate << "%" << endl;

            //Incrementing months
            time->tm_mon++;

            //Looping through every month left until staking period ends
            for (int i = 1; i <= stakingDur - 1; i++)
            {
                //Incrementing count that is outputted to csv file that show number of line
                count++;

                //Calling function to calculate amount of days during next month
                dayCalculation(time->tm_year, time->tm_mon, TempDay);

                //Calculating the amount of days until the next reward
                Tday = TempDay - rewDay;
                Tday = Tday + rewDay;

                //Checking if the month is greater than 12, if it is then it should be set to 1 and incrementing the year also
                if (time->tm_mon > 12)
                {
                    time->tm_mon = 1;
                    time->tm_year++;
                }

                //Calculating new yearly reward rate if it changes
                if (rate == "yes" && (time->tm_year == year && time->tm_mon == month + 1))
                {
                    actual = newRate / 365;
                    rewardrate = newRate;
                }

                //Calculating ongoing reward rate with previously calculated amount of days left
                ongoing = actual * Tday;

                //Calculating ethereum reward
                ethRew = eth * (ongoing / 100);

                //Reinvesting the reward
                eth = ethRew + eth;

                //Calculating total reward rate
                TotRew = TotRew + ethRew;

                //Outputting calculated data to csv file
                fs << fixed << setprecision(6) << count << ", " << time->tm_year << "-" << time->tm_mon << "-" << rewDay << ", " << eth << ", " << ethRew << ", " << TotRew << ", " << rewardrate << "%" << endl;

                //Incrementing month
                time->tm_mon++;
            }
        }
    }
    //Calculating investment if user chooses to not re-invest
    else if (reinvest == "no")
    {
        //Declaring temporary variables 
        int Tday;
        double ethRew;

        //Checking if starting day of staking is less than reward day
        if (time->tm_mday < rewDay)
        {
            //Calculating amount of days until reward is payed
            Tday = rewDay - time->tm_mday;
            //Calculating ongoing rate of reward until reward date
            ongoing = actual * Tday;
            //Calculating ETH reward of previously calculated period
            ethRew = eth * (ongoing / 100);

            //Calculating total ETH reward to date
            TotRew = TotRew + ethRew;

            //Outputting first line to csv file
            fs << fixed << setprecision(6) << count << ", " << time->tm_year << "-" << time->tm_mon << "-" << rewDay << ", " << eth << ", " << ethRew << ", " << TotRew << ", " << rewardrate << "%" << endl;

            //Incrementing month
            time->tm_mon++;

            //Looping through months until staking duration ends
            for (int i = 1; i <= stakingDur; i++)
            {
                //Incrementing line count
                count++;
                //Calculating amount of days in a given month
                dayCalculation(time->tm_year, time->tm_mon, TempDay);
                //Calculating days left until reward is payed
                Tday = TempDay - rewDay;
                Tday = Tday + rewDay;

                //Checking whether month count is greater than 12, if it then month is set to 1 and year is incremented
                if (time->tm_mon > 12)
                {
                    time->tm_mon = 1;
                    time->tm_year++;
                }

                //Calculating new yearly reward rate if it changes
                if (rate == "yes" && (time->tm_year == year && time->tm_mon == month + 1))
                {
                    actual = newRate / 365;
                    rewardrate = newRate;
                }
                //Calculating ongoing rate of a month
                ongoing = actual * Tday;
                //Calculating ETH reward with ongoing rate of a month
                ethRew = eth * (ongoing / 100);

                //Calculating total reward to date
                TotRew = TotRew + ethRew;

                //Checking if staking duration is nearing the end, if it is then output is different
                if (i < stakingDur)
                {
                    fs << fixed << setprecision(6) << count << ", " << time->tm_year << "-" << time->tm_mon << "-" << rewDay << ", " << eth << ", " << ethRew << ", " << TotRew << ", " << rewardrate << "%" << endl;
                }
                else
                {
                    fs << fixed << setprecision(6) << count << ", " << time->tm_year << "-" << time->tm_mon << "-" << time->tm_mday << ", " << eth << ", " << ethRew << ", " << TotRew << ", " << rewardrate << "%" << endl;
                }
                //Incrementing month
                time->tm_mon++;
            }
        }
        //Checking if start day is greater than reward day
        else if (time->tm_mday > rewDay)
        {
            //Incrementing month
            time->tm_mon++;

            //Checking whether month count is greater than 12, if it then month is set to 1 and year is incremented
            if (time->tm_mon > 12)
            {
                time->tm_mon = 1;
                time->tm_year++;
            }

            //Calculating new yearly reward rate if it changes
            if (rate == "yes" && (time->tm_year == year && time->tm_mon == month + 1))
            {
                actual = newRate / 365;
                rewardrate = newRate;
            }

            //Calculating amount of days in a given month
            dayCalculation(time->tm_year, time->tm_mon, TempDay);
            //Calculating amount of days until the reward is payed out
            Tday = TempDay - time->tm_mday + rewDay;
            //Calculating ongoing rate with previously calculated days
            ongoing = actual * Tday;
            //Calculating ETH reward with ongoing rate
            ethRew = eth * (ongoing / 100);

            //Calculating total reward to date
            TotRew = TotRew + ethRew;

            //Outputting first line of data to csv file
            fs << fixed << setprecision(6) << count << ", " << time->tm_year << "-" << time->tm_mon << "-" << rewDay << ", " << eth << ", " << ethRew << ", " << TotRew << ", " << rewardrate << "%" << endl;

            //Incrementing month
            time->tm_mon++;

            //Looping through months until staking duration ends
            for (int i = 1; i <= stakingDur - 1; i++)
            {
                //Incrementing line count
                count++;
                //Calculating amount of days in a given month
                dayCalculation(time->tm_year, time->tm_mon, TempDay);
                //Calculating amount of days until reward is payed
                Tday = TempDay - rewDay;
                Tday = Tday + rewDay;

                //Checking whether month count is greater than 12, if it then month is set to 1 and year is incremented
                if (time->tm_mon > 12)
                {
                    time->tm_mon = 1;
                    time->tm_year++;
                }

                //Calculating ongoing rate with previously calculated amount of days until reward is payed
                ongoing = actual * Tday;
                //Calculating ETH rate with ongoing rate
                ethRew = eth * (ongoing / 100);

                //Calculating total reward to date
                TotRew = TotRew + ethRew;

                //Outputting data to csv file
                fs << fixed << setprecision(6) << count << ", " << time->tm_year << "-" << time->tm_mon << "-" << rewDay << ", " << eth << ", " << ethRew << ", " << TotRew << ", " << rewardrate << "%" << endl;

                //Incrementing month
                time->tm_mon++;
            }
        }
    }

    //Closing file, to increase efficiency
    fs.close();
}

//Function to calculate how many days there are in a given month
void dayCalculation(int x, int y, int& z)
{
    //Checking if month is 4, 6, 9 or 11, if it is then the day count is set to 30, in every other scenario day count is set to 31, except for february
    if (y == 4 || y == 6 || y == 9 || y == 11)
    {
        z = 30;
    }
    //Checking whether the month is february
    else if (y == 2)
    {
        //Checking if it isn't leap year
        bool leapy = (x % 4 == 0 && x % 100 != 0) || (x % 400 == 0);

        //If it's not leap year the day count is set to 28, if it is leap year the day count is set to 29
        if (leapy == 0) z = 28;
        else z = 29;
    }
    else z = 31;
}
