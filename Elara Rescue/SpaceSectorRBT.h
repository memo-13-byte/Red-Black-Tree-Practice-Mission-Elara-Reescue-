#ifndef SPACESECTORRBT_H
#define SPACESECTORRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorRBT {
public:
    Sector* root;
    std::vector<Sector*> path_of_sectors;
    std::string current_path_of_sector;

    Sector* currentSector;
    std::vector<Sector*> sectorsToDelete;

    SpaceSectorRBT();
    ~SpaceSectorRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    void insertSectorByCoordinatesRecursive(Sector *current, Sector *new_sector);

    double distanceBetweenEarth(Sector *pSector1);

    std::string determineSectorCode(Sector *pSector);

    void Inorder(Sector *pSector, std::vector<Sector*> &res);

    void Preorder(Sector *pSector, std::vector<Sector*> &res);

    void Postorder(Sector *pSector, std::vector<Sector*> &res);

    void getStellarPathHelper(Sector *current, std::string &current_path, const std::string &target_sector,
                              std::vector<Sector *> &path);

    Sector* leftRotate( Sector *x);

    Sector* rightRotate( Sector *y);

    void rightLeftRotate(Sector *&root, Sector *x);

    void leftRightRotate(Sector *&root, Sector *x);

    void flipColor(Sector *node);


    void symmetric_swap(Sector *new_sector, Sector *current_sector);

    void fixInsertion(Sector *new_sector);

    Sector *findSectorRecursive(Sector *pSector, const std::string &basicString);

    void fixInsertionRecursive(Sector *node);

    bool isRed(Sector *node);

    void swapColors(Sector *node1, Sector *node2);

    void fixViolations(Sector *node);

    void getStellarPathHelper(Sector *current, Sector *target, std::vector<Sector *> &path);

    Sector *findCommonAncestor(Sector *current, Sector *target);

    void mergePathsFromCommonAncestor(std::vector<Sector *> &path, Sector *current, Sector *target);

    void mergePaths(Sector* start, Sector* end, std::vector<Sector*>& path);

    bool isAncestor(Sector *ancestor, Sector *descendant);
};

#endif // SPACESECTORRBT_H
