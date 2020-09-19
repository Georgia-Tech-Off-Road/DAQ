from PyQt5 import QtWidgets

# recursive method to find all descendants of an QtWidgets object
def findAncestryTree(ancestor_count, parent, tree):
    # If there is at least one child, continue building tree
    # If there are no children left, return the passed in tree
    if parent.findChild(QtWidgets.QWidget) != None:
        a = ""
        for n in range(ancestor_count):
            a += "\t"
        ancestor_count += 1
        for child in parent.findChildren(QtWidgets.QWidget):
            if (child.objectName() in tree) and (child.objectName() != "") or (str(child) in tree):
                pass
            else:
                if child.objectName() != "":
                    treeBranch = "\n" + a + child.objectName()
                else:
                    treeBranch = "\n" + a + str(child)

                tree = tree + findAncestryTree(ancestor_count, child, treeBranch)
    else:
        pass

    return tree