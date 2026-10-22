#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cctype>

using namespace std;

struct FileData
{
    string content;
    int characters;
    int words;
    int lines;
    bool opened;

    FileData()
    {
        content = "";
        characters = 0;
        words = 0;
        lines = 0;
        opened = false;
    }
};

// Read and analyze a file
FileData analyzeFile(string filename)
{
    FileData data;
    ifstream file(filename.c_str());

    if (!file.is_open())
    {
        cout << "\nError: Cannot open " << filename << endl;
        return data;
    }

    data.opened = true;
    string line;

    while (getline(file, line))
    {
        data.lines++;
        data.characters += (int)line.length();
        data.content += line;
        data.content += '\n';

        stringstream ss(line);
        string word;

        while (ss >> word)
        {
            data.words++;
        }
    }

    file.close();
    return data;
}

// Convert text to lowercase and remove punctuation
string cleanText(string text)
{
    string cleaned = "";

    for (int i = 0; i < (int)text.length(); i++)
    {
        unsigned char ch = (unsigned char)text[i];

        if (isalnum(ch))
        {
            cleaned += (char)tolower(ch);
        }
        else
        {
            cleaned += ' ';
        }
    }

    return cleaned;
}

// Convert content into words
vector<string> getWords(string content)
{
    vector<string> words;
    string cleaned = cleanText(content);

    stringstream ss(cleaned);
    string word;

    while (ss >> word)
    {
        words.push_back(word);
    }

    return words;
}

// Calculate word similarity
double calculateSimilarity(string content1, string content2)
{
    vector<string> words1 = getWords(content1);
    vector<string> words2 = getWords(content2);

    if (words1.empty() && words2.empty())
    {
        return 100.0;
    }

    if (words1.empty() || words2.empty())
    {
        return 0.0;
    }

    int matched = 0;
    vector<bool> used(words2.size(), false);

    for (int i = 0; i < (int)words1.size(); i++)
    {
        for (int j = 0; j < (int)words2.size(); j++)
        {
            if (!used[j] && words1[i] == words2[j])
            {
                matched++;
                used[j] = true;
                break;
            }
        }
    }

    int total = (int)words1.size();

    if ((int)words2.size() > total)
    {
        total = (int)words2.size();
    }

    return ((double)matched / total) * 100.0;
}

// Display file analysis
void displayAnalysis(string filename, FileData data)
{
    cout << "\n====================================\n";
    cout << "         FILE ANALYSIS REPORT\n";
    cout << "====================================\n";

    cout << "File Name  : " << filename << endl;
    cout << "Characters : " << data.characters << endl;
    cout << "Words      : " << data.words << endl;
    cout << "Lines      : " << data.lines << endl;

    cout << "====================================\n";
}

// Analyze one file
void analyzeOneFile()
{
    string filename;

    cout << "\nEnter file name (example: file1.txt): ";
    cin >> filename;

    FileData data = analyzeFile(filename);

    if (data.opened)
    {
        displayAnalysis(filename, data);
    }
}

// Compare two files
void compareFiles()
{
    string file1, file2;

    cout << "\nEnter first file name: ";
    cin >> file1;

    cout << "Enter second file name: ";
    cin >> file2;

    FileData data1 = analyzeFile(file1);
    FileData data2 = analyzeFile(file2);

    if (!data1.opened || !data2.opened)
    {
        cout << "\nComparison failed. Check the file names.\n";
        return;
    }

    if (data1.words == 0 && data2.words == 0)
    {
        cout << "\nBoth files are empty. Similarity cannot be meaningfully determined.\n";
        return;
    }

    double similarity =
        calculateSimilarity(data1.content, data2.content);

    cout << fixed << setprecision(2);

    cout << "\n====================================\n";
    cout << "     DUPLICATE CONTENT ANALYSIS\n";
    cout << "====================================\n";

    cout << "File 1     : " << file1 << endl;
    cout << "File 2     : " << file2 << endl;
    cout << "Words in file 1: " << data1.words << endl;
    cout << "Words in file 2: " << data2.words << endl;
    cout << "Similarity : " << similarity << "%\n";

    if (similarity >= 90.0)
    {
        cout << "Status     : HIGHLY SIMILAR CONTENT\n";
    }
    else if (similarity >= 50.0)
    {
        cout << "Status     : PARTIALLY SIMILAR CONTENT\n";
    }
    else
    {
        cout << "Status     : LOW SIMILARITY\n";
    }

    cout << "====================================\n";
}

// Generate report
void generateReport()
{
    string filename;

    cout << "\nEnter file name for report: ";
    cin >> filename;

    FileData data = analyzeFile(filename);

    if (!data.opened)
    {
        cout << "\nReport could not be generated.\n";
        return;
    }

    ofstream report("analysis_report.txt");

    if (!report.is_open())
    {
        cout << "\nError: Cannot create report file.\n";
        return;
    }

    report << "====================================\n";
    report << "         FILE ANALYSIS REPORT\n";
    report << "====================================\n";

    report << "File Name  : " << filename << endl;
    report << "Characters : " << data.characters << endl;
    report << "Words      : " << data.words << endl;
    report << "Lines      : " << data.lines << endl;

    report << "====================================\n";

    report.close();

    cout << "\nReport generated successfully!\n";
    cout << "Saved as analysis_report.txt\n";
}

// Main program
int main()
{
    int choice;

    do
    {
        cout << "\n\n";
        cout << "============================================\n";
        cout << " DUPLICATE CONTENT DETECTION SYSTEM\n";
        cout << "============================================\n";

        cout << "1. Analyze a File\n";
        cout << "2. Compare Two Files\n";
        cout << "3. Generate Analysis Report\n";
        cout << "4. Exit\n";

        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cout << "\nInvalid input. Please enter a number.\n";
            return 0;
        }

        switch (choice)
        {
        case 1:
            analyzeOneFile();
            break;

        case 2:
            compareFiles();
            break;

        case 3:
            generateReport();
            break;

        case 4:
            cout << "\nThank you for using the system!\n";
            break;

        default:
            cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}