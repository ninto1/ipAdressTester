#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;
string exec(string command)
{
   char buffer[128];
   string result = "";

   // Open pipe to file
   FILE *pipe = popen(command.c_str(), "r");
   if (!pipe)
   {
      return "popen failed!";
   }

   // read till end of process:
   while (!feof(pipe))
   {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}
string getIp(int parts)
{
   string result = "";
   int num = 0;
   for (int i = 0; i < parts; i++)
   {
      printf("Please enter the %d. part of the IP:", i + 1);
      scanf("%d", &num);
      stringstream ss;
      ss << num;
      string str = ss.str();
      result += str + ".";
   }
   return result;
}
void writeResultsToFile(int replys[], int currentRep, string ip)
{
   string filename = ip + "x.dat";
   char fileNameC[17] = {'a'};
   strcpy(fileNameC, filename.c_str());
   FILE *fp = fopen(fileNameC, "w");
   for (int i = 0; i < currentRep; i++)
   {
      string buff = ip;
      for (int k = 0; k < buff.length(); k++)
      { // DEBUG ONLY
         printf("%c", buff[k]);
         fprintf(fp, "%c", buff[k]);
      }
      printf("%d\n", replys[i]);
      fprintf(fp, "%d\n", replys[i]);
   }
   fclose(fp);
}
void ipTester(string ip)
{
   int replys[255] = {0};
   int currentRep = 0;
   string ls;
   for (int i = 1; i < 254; i++)
   {
      stringstream ss;
      ss << i;
      string str = ss.str();
      string buff = ip;
      ls = exec("ping " + buff + str + " -n 1 -w 500");
      if (!(ls.find("Request timed out.") != std::string::npos))
      {
         cout << ip;
         cout << str;
         printf(" succeeded\n", i);
         replys[currentRep] = i;
         currentRep++;
      }
      else
      {
         cout << ip;
         cout << str;
         printf(" failed\n", i);
      }
   }
   writeResultsToFile(replys, currentRep, ip);
}
void ipTestLoop(int ttl, string ip)
{
   if (ttl > 1)
      for (int i = 1; i < 254; i++)
      {
         stringstream ss;
         ss << i;
         string str = ss.str();
         string buff = ip;
         buff += str;
         buff += ".";
         ipTestLoop(ttl - 1, buff);
      }
   else
   {
      ipTester(ip);
   }
}
int main()
{
   printf("How many parts of the IP to input?");
   int ipParts = 0;
   while (ipParts < 1 || ipParts > 3)
      scanf("%d", &ipParts);
   string ip = getIp(ipParts);
   ipTestLoop(4 - ipParts, ip);

   int a;
   scanf("%d", &a);
}