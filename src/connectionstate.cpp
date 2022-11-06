#include "connectionstate.h"

#include <iostream>
#include <QPointF>

#include "flowscene.h"
#include "node.h"

ConnectionState::~ConnectionState()
{
    resetLastHoveredNode();
}

void ConnectionState::interactWithNode(Node *node)
{
    if (node) {
        _lastHoveredNode = node;
    } else {
        resetLastHoveredNode();
    }
}

void ConnectionState::setLastHoveredNode(Node *node)
{
    _lastHoveredNode = node;
}

void ConnectionState::resetLastHoveredNode()
{
    if (_lastHoveredNode)
//        _lastHoveredNode->resetReactionToConnection();

    _lastHoveredNode = nullptr;
}
