//============================================================================
// ProjectTwo.cpp
// CS-300 Project Two
// Name: Nikita Simon
// ABCU Advising Assistance Program
//============================================================================

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//============================================================================
// Course Structure
//============================================================================

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

//============================================================================
// Binary Search Tree Node
//============================================================================

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

//============================================================================
// Binary Search Tree Class
//============================================================================

class BinarySearchTree {
private:
    Node* root;

    void insert(Node*& node, Course course);
    void inOrder(Node* node);
    Course search(Node* node, string courseNumber);
    void destroyTree(Node* node);

public:
    BinarySearchTree();
    ~BinarySearchTree();

    void insert(Course course);
    void printCourseList();
    Course search(string courseNumber);
};

//============================================================================
// Function Prototypes
//============================================================================

bool loadCourses(string fileName, BinarySearchTree& tree);
string toUpperCase(string text);
void displayCourse(Course course, BinarySearchTree& tree);

//============================================================================
// Binary Search Tree Implementation
//============================================================================

BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

BinarySearchTree::~BinarySearchTree() {
    destroyTree(root);
}

void BinarySearchTree::destroyTree(Node* node) {

    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

void BinarySearchTree::insert(Course course) {
    insert(root, course);
}

void BinarySearchTree::insert(Node*& node, Course course) {

    if (node == nullptr) {
        node = new Node(course);
    }
    else if (course.courseNumber < node->course.courseNumber) {
        insert(node->left, course);
    }
    else {
        insert(node->right, course);
    }
}

void BinarySearchTree::printCourseList() {
    inOrder(root);
}

void BinarySearchTree::inOrder(Node* node) {

    if (node == nullptr) {
        return;
    }

    inOrder(node->left);

    cout << node->course.courseNumber
         << ", "
         << node->course.courseTitle
         << endl;

    inOrder(node->right);
}

Course BinarySearchTree::search(string courseNumber) {
    return search(root, toUpperCase(courseNumber));
}

Course BinarySearchTree::search(Node* node, string courseNumber) {

    if (node == nullptr) {
        return Course();
    }

    if (node->course.courseNumber == courseNumber) {
        return node->course;
    }

    if (courseNumber < node->course.courseNumber) {
        return search(node->left, courseNumber);
    }

    return search(node->right, courseNumber);
}

//============================================================================
// Helper Functions
//============================================================================

// Convert a string to uppercase
string toUpperCase(string text) {

    transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

// Display a course and its prerequisites
void displayCourse(Course course, BinarySearchTree& tree) {

    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << endl;
    cout << course.courseNumber << ", "
         << course.courseTitle << endl;

   cout << "Prerequisites: ";

if (course.prerequisites.empty()) {

    cout << "None";
}
else {

    for (size_t i = 0; i < course.prerequisites.size(); i++) {

        Course prereq = tree.search(course.prerequisites[i]);

        cout << prereq.courseNumber;

        if (!prereq.courseTitle.empty()) {
            cout << ", " << prereq.courseTitle;
        }

                if (i < course.prerequisites.size() - 1) {
            cout << ", ";
        }
    }
}

cout << endl << endl;
}

//============================================================================
// Load Course Data
//============================================================================

bool loadCourses(string fileName, BinarySearchTree& tree) {

    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return false;
    }

    vector<Course> courses;
    vector<string> validCourseNumbers;

    string line;

    // Read every line from the file
    while (getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string token;

        vector<string> tokens;

        while (getline(ss, token, ',')) {

    // Remove carriage return if present
    token.erase(remove(token.begin(), token.end(), '\r'), token.end());

    tokens.push_back(token);
}

        // Every course must contain at least
        // a course number and title
        if (tokens.size() < 2) {

            cout << "Invalid file format." << endl;

            file.close();
            return false;
        }

        Course course;

course.courseNumber = toUpperCase(tokens[0]);
        course.courseTitle = tokens[1];
course.courseTitle.erase(
    remove(course.courseTitle.begin(),
           course.courseTitle.end(),
           '\r'),
    course.courseTitle.end());

        validCourseNumbers.push_back(course.courseNumber);

        for (size_t i = 2; i < tokens.size(); i++) {

            if (!tokens[i].empty()) {
                course.prerequisites.push_back(toUpperCase(tokens[i]));
            }
        }

        courses.push_back(course);
    }

    file.close();

    // Validate prerequisites
    for (Course& course : courses) {

        for (string prerequisite : course.prerequisites) {

            bool found = false;

            for (string courseNumber : validCourseNumbers) {

            

if (courseNumber == prerequisite) {
                    found = true;
                    break;
                }
            }

            if (!found) {

                cout << "Error: Invalid prerequisite "
                     << prerequisite
                     << " for "
                     << course.courseNumber
                     << endl;

                return false;
            }
        }
    }

    // Insert courses into the Binary Search Tree
    for (Course course : courses) {
        tree.insert(course);
    }

    cout << endl;
    cout << "Course data loaded successfully." << endl;

    return true;
}

//============================================================================
// Main Program
//============================================================================

int main() {

    BinarySearchTree courseTree;

    int choice = 0;
    bool dataLoaded = false;
    string fileName;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {

        cout << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        switch (choice) {

        case 1: {

            cout << endl;
            cout << "Enter the file name: ";
            cin >> fileName;

            dataLoaded = loadCourses(fileName, courseTree);

            break;
        }

        case 2: {

            if (!dataLoaded) {
                cout << endl;
                cout << "Please load the course data first." << endl;
            }
            else {
                cout << endl;
                cout << "Here is a sample schedule:" << endl;
                cout << endl;

                courseTree.printCourseList();
            }

            break;
        }

        case 3: {

            if (!dataLoaded) {
                cout << endl;
                cout << "Please load the course data first." << endl;
            }
            else {

                string courseNumber;

                cout << endl;
                cout << "What course do you want to know about? ";
                cin >> courseNumber;

                Course course = courseTree.search(courseNumber);

                displayCourse(course, courseTree);
            }

            break;
        }

        case 9:

            cout << endl;
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:

            cout << endl;
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}