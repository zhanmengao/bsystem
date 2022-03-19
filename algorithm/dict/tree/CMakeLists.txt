cmake_minimum_required(VERSION 3.5)

project(dict LANGUAGES CXX)
project(rbtree LANGUAGES CXX)
project(redis LANGUAGES C)
include_directories(../../queue/cpp)
include_directories(../../list/Liner)
include_directories(../../sort/cpp)

add_executable(tree BST.cpp BinaryTree.h BST.h BT.h  BinarySearchTree.h TreeUtils.h tree.cpp)

add_executable(rbtree RedBlackTree.h rbt.cpp AdelsonVelskiiLandisTree.h Tree23.h)
