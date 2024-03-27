#include <valarray>
#include "SpaceSectorRBT.h"
#include <stack>
#include <unordered_set>

using namespace std;

SpaceSectorRBT::SpaceSectorRBT() : root(nullptr) {}

void SpaceSectorRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.

    ifstream file(filename, ios::in);
    if (file.is_open()) {
        string line;
        getline(file, line);
        while (getline(file, line)) {
            stringstream ss(line);
            char comma;
            int x = 0;
            int y = 0;
            int z = 0;
            ss >> x >> comma >> y >> comma >> z;
            insertSectorByCoordinates(x, y, z);
        }
    }

}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorRBT::~SpaceSectorRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    int size = sectorsToDelete.size();
    for (int i = 0; i < size; i++) {
        delete sectorsToDelete[i];
    }

    sectorsToDelete.clear();
}

void SpaceSectorRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.

    Sector* new_sector = new Sector(x, y, z);
    new_sector->left = nullptr;
    new_sector->right = nullptr;
    new_sector->parent = nullptr;
    if (root == nullptr) {
        root = new_sector;
        root->color = BLACK;
        root->distance_from_earth = distanceBetweenEarth(root);
        root->sector_code = determineSectorCode(root);
        sectorsToDelete.push_back(root);
    }

    else {
        new_sector->color = RED;
        new_sector->distance_from_earth = distanceBetweenEarth(new_sector);
        new_sector->sector_code = determineSectorCode(new_sector);
        sectorsToDelete.push_back(new_sector);
        insertSectorByCoordinatesRecursive(root, new_sector);
        fixViolations(new_sector);

//        fixInsertionRecursive(new_sector);  // Call fixInsertion after insertion


    }
}

void SpaceSectorRBT::insertSectorByCoordinatesRecursive(Sector* current, Sector* new_sector) {



    if (current == nullptr || new_sector == nullptr) {
        // Handle nullptr case as needed
        return;
    }

    // Check for member access and types
    if (new_sector->x == current->x) {
        if (new_sector->y == current->y) {
            if (new_sector->z == current->z) {
                // Handle the case when the sector is already present
                // delete new_sector; // You might want to reconsider deleting new_sector here
                return;
            } else if (new_sector->z < current->z) {
                // Check left child
                if (current->left == nullptr) {
                    current->left = new_sector;
                    new_sector->parent = current;


                } else {
                    // Recursively insert on the left
                    insertSectorByCoordinatesRecursive(current->left, new_sector);
                }
            } else {
                // Check right child
                if (current->right == nullptr) {
                    current->right = new_sector;
                    new_sector->parent = current;

                } else {
                    // Recursively insert on the right
                    insertSectorByCoordinatesRecursive(current->right, new_sector);
                }
            }
        } else if (new_sector->y < current->y) {
            // Check left child
            if (current->left == nullptr) {
                current->left = new_sector;
                new_sector->parent = current;

            } else {
                // Recursively insert on the left
                insertSectorByCoordinatesRecursive(current->left, new_sector);
            }
        } else {
            // Check right child
            if (current->right == nullptr) {
                current->right = new_sector;
                new_sector->parent = current;

            } else {
                // Recursively insert on the right
                insertSectorByCoordinatesRecursive(current->right, new_sector);
            }
        }
    } else if (new_sector->x < current->x) {
        // Check left child
        if (current->left == nullptr) {
            current->left = new_sector;
            new_sector->parent = current;

        } else {
            // Recursively insert on the left
            insertSectorByCoordinatesRecursive(current->left, new_sector);
        }
    } else {
        // Check right child
        if (current->right == nullptr) {
            current->right = new_sector;
            new_sector->parent = current;

        } else {
            // Recursively insert on the right
            insertSectorByCoordinatesRecursive(current->right, new_sector);
        }
    }


}


void SpaceSectorRBT::fixViolations(Sector* node) {
    while (node != nullptr && node->parent != nullptr && isRed(node->parent)) {
        if (node->parent == node->parent->parent->left) {
            Sector* uncle = node->parent->parent->right;

            if (isRed(uncle)) {
                // Case 1: Uncle is RED
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    // Case 2: Uncle is BLACK, and node is a right child
                    node = node->parent;
                    leftRotate(node);
                }

                // Case 3: Uncle is BLACK, and node is a left child
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rightRotate(node->parent->parent);
            }
        } else {
            // Similar logic as above with "left" and "right" swapped
            Sector* uncle = node->parent->parent->left;

            if (isRed(uncle)) {
                // Case 1: Uncle is RED
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    // Case 2: Uncle is BLACK, and node is a left child
                    node = node->parent;
                    rightRotate(node);
                }

                // Case 3: Uncle is BLACK, and node is a right child
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                leftRotate(node->parent->parent);
            }
        }
    }

    // Ensure the root is black
    root->color = BLACK;
}

void SpaceSectorRBT::fixInsertion(Sector *new_sector) {

    if (new_sector->parent == nullptr) {
        new_sector->color = BLACK;  // Root is always black
        return;
    }

    fixInsertionRecursive(new_sector);

}

void SpaceSectorRBT::fixInsertionRecursive(Sector *new_sector) {
    if(new_sector->parent== nullptr){
        new_sector->color=BLACK;
        return;
    }

    if(new_sector->parent->color==BLACK){
        return;
    }

    if(new_sector->parent->color==RED){
        if(new_sector->uncle()== nullptr || new_sector->uncle()->color==BLACK){
            if(new_sector->parent->isLeftChild()){
                if(new_sector->isLeftChild()){
                    rightRotate(new_sector->parent->parent);
                    new_sector->parent->color=BLACK;
                    new_sector->parent->right->color=RED;
                }
                else{
                    leftRotate(new_sector->parent);
                    rightRotate(new_sector->parent);
                    new_sector->color=BLACK;
                    new_sector->right->color=RED;
                }
            }
            else{
                if(new_sector->isLeftChild()){
                    rightRotate(new_sector->parent);
                    leftRotate(new_sector->parent);
                    new_sector->color=BLACK;
                    new_sector->left->color=RED;
                }
                else{
                    leftRotate(new_sector->parent->parent);
                    new_sector->parent->color=BLACK;
                    new_sector->parent->left->color=RED;
                }
            }
        }
        else{
            new_sector->parent->color=BLACK;
            new_sector->uncle()->color=BLACK;
            new_sector->parent->parent->color=RED;
            fixInsertionRecursive(new_sector->parent);
        }
    }




}




void SpaceSectorRBT::swapColors(Sector *node1, Sector *node2) {
    bool temp = node1->color;
    node1->color = node2->color;
    node2->color = temp;
}

void SpaceSectorRBT::symmetric_swap(Sector *new_sector, Sector *grand_parent) {// Symmetric case for left and right swapped




    if (new_sector == nullptr) {
        // Handle the case when the input sector is null
        return;
    }

    if (new_sector->parent != nullptr) {
        Sector *parent = new_sector->parent;

        if (parent->left == new_sector) {
            // Swap left and right children
            Sector *temp = new_sector->left;
            new_sector->left = new_sector->right;
            new_sector->right = temp;

            // Update parent pointers
            if (new_sector->left != nullptr) {
                new_sector->left->parent = new_sector;
            }
            if (new_sector->right != nullptr) {
                new_sector->right->parent = new_sector;
            }

            // Update parent's child pointer
            parent->left = new_sector;
        } else if (parent->right == new_sector) {
            // Swap left and right children
            Sector *temp = new_sector->left;
            new_sector->left = new_sector->right;
            new_sector->right = temp;

            // Update parent pointers
            if (new_sector->left != nullptr) {
                new_sector->left->parent = new_sector;
            }
            if (new_sector->right != nullptr) {
                new_sector->right->parent = new_sector;
            }

            // Update parent's child pointer
            parent->right = new_sector;
        }
    } else if (grand_parent != nullptr) {
        // Handle the case when new_sector->parent is nullptr
        // Update the grandparent's child pointer
        if (grand_parent->left == new_sector) {
            grand_parent->left = new_sector->left;
            if (new_sector->left != nullptr) {
                new_sector->left->parent = grand_parent;
            }
            new_sector->left = nullptr;
        } else if (grand_parent->right == new_sector) {
            grand_parent->right = new_sector->left;
            if (new_sector->left != nullptr) {
                new_sector->left->parent = grand_parent;
            }
            new_sector->left = nullptr;
        }
    } else {
        // Handle the case when new_sector->parent is nullptr
        // You may want to log an error or handle this situation appropriately
        return;
    }


}


Sector* SpaceSectorRBT::leftRotate(Sector* node) {
    if(node == nullptr)
        return nullptr;
    if(node->right == nullptr)
        return nullptr;
    Sector* right_child = node->right;
    node->right = right_child->left;

    if (node->right != nullptr)
        node->right->parent = node;

    right_child->parent = node->parent;
    node->color= true;
    right_child->color= false;
    if (node->parent == nullptr){
        root=right_child;
    }
    else if (node == node->parent->left)
        node->parent->left = right_child;
    else
        node->parent->right = right_child;

    right_child->left = node;
    node->parent = right_child;

     // Return the new root
     return right_child;
}

// Right rotation
Sector* SpaceSectorRBT::rightRotate( Sector* node) {
    if(node == nullptr)
        return nullptr;
    if(node->left == nullptr)
        return nullptr;
    Sector* left_child = node->left;
    node->left = left_child->right;

    if (node->left != nullptr)
        node->left->parent = node;

    left_child->parent = node->parent;
    node->color= true;
    left_child->color= false;
    if (node->parent == nullptr)
        root=left_child;
    else if (node == node->parent->left)
        node->parent->left = left_child;
    else
        node->parent->right = left_child;

    left_child->right = node;
    node->parent = left_child;

    return left_child;

    // Return the new root
}
// Left-right rotation
//void SpaceSectorLLRBT::leftRightRotate(Sector*& root, Sector* grand_parent) {
//    leftRotate(root, grand_parent->left);
//    // Right rotation on grand_parent
//    rightRotate(root, grand_parent);
//}
//
////// Right-left rotation
//void SpaceSectorLLRBT::rightLeftRotate(Sector*& root, Sector* grand_parent) {
//    rightRotate(root, grand_parent->right);
//    // Left rotation on grand_parent
//    leftRotate(root, grand_parent);
//}


void SpaceSectorRBT::flipColor(Sector* node) {

    if (!isRed(node) || !isRed(node->left) || !isRed(node->right)) {
        // Handle the error condition here
        // For example, you can throw an exception or print an error message
//        std::cerr << "Invalid red-black tree node colors" << std::endl;
    } else {
        node->color = RED;
        node->left->color = BLACK;
        node->right->color = BLACK;
    }
}

bool SpaceSectorRBT::isRed(Sector* node) {
    return (node != nullptr) && (node->color == RED);
}



double SpaceSectorRBT::distanceBetweenEarth(Sector *pSector1) {
    return sqrt(pow(pSector1->x, 2) + pow(pSector1->y, 2) + pow(pSector1->z, 2));
}

std::string SpaceSectorRBT::determineSectorCode(Sector *pSector) {
    string sector_code = "";

    sector_code += to_string(static_cast<int>(pSector->distance_from_earth));
    if (pSector->x < 0) {
        sector_code += "L";
    }
    else if(pSector->x > 0){
        sector_code += "R";
    }

    else {
        sector_code += "S";
    }

    if (pSector->y < 0) {
        sector_code += "D";
    }
    else if(pSector->y > 0){
        sector_code += "U";
    }
    else {
        sector_code += "S";
    }

    if (pSector->z < 0) {
        sector_code += "B";
    }
    else if(pSector->z > 0){
        sector_code += "F";
    }
    else {
        sector_code += "S";
    }

    return sector_code;
}


void SpaceSectorRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    vector<Sector*> res;
    Inorder(root, res);
    std::cout << "Space sectors inorder traversal: " << std::endl;
    for (int i = 0; i < res.size(); i++) {
        std::string sector_color = (res[i]->color) ? "RED sector: "  : "BLACK sector: ";
        cout << sector_color;
        cout << res[i]->sector_code << endl;
    }

    cout << endl;
}

void SpaceSectorRBT::Inorder(Sector *pSector, vector<Sector*> &res) {

    if (pSector == nullptr) {
        return;
    }

    Inorder(pSector->left, res);
    res.push_back(pSector);
    Inorder(pSector->right, res);
}


void SpaceSectorRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::vector<Sector*> res;
    Preorder(root, res);
    std::cout << "Space sectors preorder traversal: " << std::endl;
    for (int i = 0; i < res.size(); i++) {
        std::string sector_color = (res[i]->color) ? "RED sector: "  : "BLACK sector: ";
        cout << sector_color;
        cout << res[i]->sector_code << endl;
    }

    cout << endl;
}

void SpaceSectorRBT::Preorder(Sector *pSector, vector<Sector*> &res) {
    if (pSector == nullptr) {
        return;
    }

    res.push_back(pSector);
    Preorder(pSector->left, res);
    Preorder(pSector->right, res);
}

void SpaceSectorRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::vector<Sector*> res;
    Postorder(root, res);
    std::cout << "Space sectors postorder traversal: " << std::endl;
    for (int i = 0; i < res.size(); i++) {
        std::string sector_color = (res[i]->color) ? "RED sector: "  : "BLACK sector: ";
        cout << sector_color;
        cout << res[i]->sector_code << endl;
    }

    cout << endl;
}


void SpaceSectorRBT::Postorder(Sector *pSector, vector<Sector*> &res) {
    if (pSector == nullptr) {
        return;
    }

    Postorder(pSector->left, res);
    Postorder(pSector->right, res);
    res.push_back(pSector);
}

std::vector<Sector*> SpaceSectorRBT::getStellarPath(const std::string& sector_code) {
    path_of_sectors.clear();
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    Sector* target = findSectorRecursive(root, sector_code);
    target = currentSector;
    currentSector = nullptr;
    Sector* startSector = findSectorRecursive(root, "0SSS");
    startSector = currentSector;
    currentSector = nullptr;
//    getStellarPathHelper(target, current_path_of_sector, sector_code, path_of_sectors);
    if(target == startSector) {
        path_of_sectors.push_back(startSector);
        return path_of_sectors;
    }
    else {
        if (startSector != nullptr && target != nullptr) {
            Sector *commonAncestor = findCommonAncestor(startSector, target);

            if (commonAncestor == nullptr) {
                // Ortak atama bulunamazsa, iki yolu birleştir
                mergePaths(startSector, target, path_of_sectors);
            } else if (commonAncestor == startSector) {
                while (target != nullptr && target != commonAncestor) {
                    path_of_sectors.push_back(target);
                    target = target->getParent();
                }
                if (path_of_sectors.back() != commonAncestor) {
                    path_of_sectors.push_back(commonAncestor);
                }
                std::reverse(path_of_sectors.begin(), path_of_sectors.end());
            } else if (commonAncestor == target) {
                while (startSector != nullptr && startSector != commonAncestor) {
                    path_of_sectors.push_back(startSector);
                    startSector = startSector->getParent();
                }
                if (path_of_sectors.back() != commonAncestor) {
                    path_of_sectors.push_back(commonAncestor);
                }
            } else {
//            // Ortak atama bulunduysa, yolları birleştir
                mergePaths(startSector, target, path_of_sectors);
//            mergePaths(target, commonAncestor, path_of_sectors);
            }
        }

        return path_of_sectors;
    }
}

void SpaceSectorRBT::mergePaths(Sector* start, Sector* end, std::vector<Sector*>& path) {
    std::vector<Sector*> tempPath1;
    std::vector<Sector*> tempPath2;

    Sector* current = start;
    Sector* commonAncestor = findCommonAncestor(start, end);


    // Start'tan ortak ataya kadar olan yolu ekleyin
    while (current != nullptr && current != commonAncestor) {
        tempPath1.push_back(current);
        current = current->getParent();
    }

    // Ortak atadan end'e kadar olan yolu ekleyin
    current = end;
    while (current != nullptr && current != commonAncestor) {
        tempPath2.push_back(current);
        current = current->getParent();
    }


    for (size_t i = 0; i < tempPath1.size(); ++i) {
        path.push_back(tempPath1[i]);
    }
    path.push_back(commonAncestor);

    // Yolu birleştirin
    for (int i = tempPath2.size() - 1; i >= 0; --i) {
        path.push_back(tempPath2[i]);
    }

}


Sector* SpaceSectorRBT::findCommonAncestor(Sector* node1, Sector* node2) {
    // Bu fonksiyon, iki nodun ilk ortak atalarını bulur
    std::unordered_set<Sector*> ancestors;

    // node1'in atalarını set'e ekleyin
    while (node1 != nullptr) {
        ancestors.insert(node1);
        node1 = node1->getParent();
    }

    // node2'nin atalarını kontrol edin
    while (node2 != nullptr) {
        // İlk ortak atayı bulduysak, onu döndürün
        if (ancestors.count(node2) > 0) {
            return node2;
        }
        node2 = node2->getParent();
    }

    return nullptr; // Ortak atama bulunamadı
}





bool SpaceSectorRBT::isAncestor(Sector* ancestor, Sector* descendant) {
    // descendant, ancestor'ın altında mı kontrol et
    while (descendant != nullptr) {
        if (descendant == ancestor) {
            return true;
        }
        descendant = descendant->getParent();
    }
    return false;
}






























void SpaceSectorRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
    if(!current_path_of_sector.empty()) {
        std::cout << "The stellar path to Dr. Elara: " << current_path_of_sector << std::endl;

        current_path_of_sector.clear();
        path_of_sectors.clear();
    }

    else {
        std::cout << "A path to Dr. Elara could not found." << std::endl;
    }
}

Sector *SpaceSectorRBT::findSectorRecursive(Sector *pSector, const string &basicString) {
    if (pSector == nullptr) {
        return nullptr;
    }

    if (pSector->sector_code == basicString) {
        currentSector = pSector;
        return pSector;
    }

    Sector* left = findSectorRecursive(pSector->left, basicString);
    Sector* right = findSectorRecursive(pSector->right, basicString);

    return nullptr;
}

