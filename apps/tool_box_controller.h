#ifndef APPS_TOOL_BOX_CONTROLLER_H
#define APPS_TOOL_BOX_CONTROLLER_H

#include <escher.h>
#include "node_navigation_controller.h"

class ToolBoxController : public NodeNavigationController {
public:
  const char * title() const override;
private:
  Node * nodeModel() override;
  bool selectLeaf(Node * selectedNode) override;
};

#endif