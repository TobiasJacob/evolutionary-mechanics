#include "Plotter.hpp"

string head = "<!DOCTYPE html>\n<html>\n<head>\n  <meta charset=\"utf-8\"/>\n  <title>Evolutionary mechanics</title>\n  <script type=\"text/javascript\">\n    function draw() {\n      // Paste output here\n";
string tail = "        \n        // Paste output here\n        var canvas = document.getElementById('tutorial');\n        var ctx = canvas.getContext('2d');\n        ctx.fillStyle = \"white\";\n        ctx.fillRect(0, 0, canvas.width, canvas.height);\n\n        const rows = field.length;\n        const cols = field[0].length;\n        const offsetX = 100;\n        const offsetY = 100;\n        \n        const rectSize = 800 / Math.max(rows, cols);\n        const movementScale = 1;\n\n        function coordsToView(x, y) {\n          return [offsetX + rectSize * x, offsetY + rectSize * y];\n        }\n\n        let stressI = 0;\n        const stressScale = 255 / Math.max(...stress);\n        ctx.strokeStyle = '#000';\n        for (let r = 0; r < rows; r++) {\n          for (let c = 0; c < cols; c++) {\n            if (field[r][c]) {\n              ctx.beginPath();\n              ctx.moveTo(...coordsToView(r + movementScale * u[cornerIndexRow[r][c]], c + movementScale * u[cornerIndexCol[r][c]]));\n              ctx.lineTo(...coordsToView((r + 1) + movementScale * u[cornerIndexRow[r + 1][c]], c + movementScale * u[cornerIndexCol[r + 1][c]]));\n              ctx.lineTo(...coordsToView((r + 1) + movementScale * u[cornerIndexRow[r + 1][c + 1]], (c + 1) + movementScale * u[cornerIndexCol[r + 1][c + 1]]));\n              ctx.lineTo(...coordsToView(r + movementScale * u[cornerIndexRow[r][c + 1]], (c + 1) + movementScale * u[cornerIndexCol[r][c + 1]]));\n              ctx.closePath();\n              ctx.stroke();\n\n              ctx.fillStyle = 'rgb(255, ' + (255 - stress[stressI] * stressScale) + ', 255)';\n              ctx.beginPath();\n              ctx.moveTo(...coordsToView(r + movementScale * u[cornerIndexRow[r][c]], c + movementScale * u[cornerIndexCol[r][c]]));\n              ctx.lineTo(...coordsToView((r + 1) + movementScale * u[cornerIndexRow[r + 1][c]], c + movementScale * u[cornerIndexCol[r + 1][c]]));\n              ctx.lineTo(...coordsToView((r + 1) + movementScale * u[cornerIndexRow[r + 1][c + 1]], (c + 1) + movementScale * u[cornerIndexCol[r + 1][c + 1]]));\n              ctx.closePath();\n              ctx.fill();\n\n              ctx.fillStyle = 'rgb(255, ' + (255 - stress[stressI + 1] * stressScale) + ', 255)';\n              ctx.beginPath();\n              ctx.moveTo(...coordsToView((r + 1) + movementScale * u[cornerIndexRow[r + 1][c + 1]], (c + 1) + movementScale * u[cornerIndexCol[r + 1][c + 1]]));\n              ctx.lineTo(...coordsToView(r + movementScale * u[cornerIndexRow[r][c + 1]], (c + 1) + movementScale * u[cornerIndexCol[r][c + 1]]));\n              ctx.lineTo(...coordsToView(r + movementScale * u[cornerIndexRow[r][c]], c + movementScale * u[cornerIndexCol[r][c]]));\n              ctx.closePath();\n              ctx.fill();\n\n              stressI += 2;\n            }\n          }\n        }\n\n        ctx.strokeStyle = '#C00';\n        const supportSize = 0.2;\n        ctx.beginPath();\n        ctx.moveTo(...coordsToView(supports.SupportRow1.row, supports.SupportRow1.col - supportSize));\n        ctx.lineTo(...coordsToView(supports.SupportRow1.row, supports.SupportRow1.col + supportSize));\n        ctx.closePath();\n        ctx.stroke();\n        ctx.beginPath();\n        ctx.moveTo(...coordsToView(supports.SupportRow2.row, supports.SupportRow2.col - supportSize));\n        ctx.lineTo(...coordsToView(supports.SupportRow2.row, supports.SupportRow2.col + supportSize));\n        ctx.closePath();\n        ctx.stroke();\n        ctx.beginPath();\n        ctx.moveTo(...coordsToView(supports.SupportCol.row - supportSize, supports.SupportCol.col));\n        ctx.lineTo(...coordsToView(supports.SupportCol.row + supportSize, supports.SupportCol.col));\n        ctx.closePath();\n        ctx.stroke();\n\n        function arrow(fromx, fromy, tox, toy) { // https://stackoverflow.com/questions/808826/draw-arrow-on-canvas-tag\n          var headlen = 10; // length of head in pixels\n          var dx = tox - fromx;\n          var dy = toy - fromy;\n          var angle = Math.atan2(dy, dx);\n          ctx.moveTo(fromx, fromy);\n          ctx.lineTo(tox, toy);\n          ctx.lineTo(tox - headlen * Math.cos(angle - Math.PI / 6), toy - headlen * Math.sin(angle - Math.PI / 6));\n          ctx.moveTo(tox, toy);\n          ctx.lineTo(tox - headlen * Math.cos(angle + Math.PI / 6), toy - headlen * Math.sin(angle + Math.PI / 6));\n        }\n\n        ctx.strokeStyle = '#070';\n        const forceSize = 6;\n        for (let i = 0; i < forces.length; i++) {\n          const force = forces[i];\n          const r = force.attackCorner.row;\n          const c = force.attackCorner.col;\n          const [x, y] = coordsToView(r + movementScale * u[cornerIndexRow[r][c]], c + movementScale * u[cornerIndexCol[r][c]]);\n          ctx.beginPath();\n          arrow(x, y, x + force.forceRow * forceSize, y + force.forceCol * forceSize);\n          ctx.closePath();\n          ctx.stroke();\n        }\n\n        var statusP = document.getElementById('statusText');\n        statusP.innerHTML = \"Converged after \" + solutionSteps + \" with residum \" + residum;\n      }\n    </script>\n    <style type=\"text/css\">\n    </style>\n  </head>\n  <body onload=\"draw();\">\n    <canvas id=\"tutorial\" width=\"1000\" height=\"1000\"></canvas>\n    <p id=\"statusText\">Converged after ...</p>\n  </body>\n</html>";

Plotter::Plotter(string fileName): output(fileName)
{
}

// Requires not empty fields, etc.
void Plotter::plot(const Field &field, const vector<float> &u, const Matrix<int> &cornerIndexRow, const Matrix<int> &cornerIndexCol, const Support &supports, const vector<Force> &forces, const int solutionSteps, const float residum, const vector<float> &stress) 
{
    // head
    output << head;

    // field
    output << "const field = [";
    output << "[" << field.Plane(0, 0);
    for (int c = 1; c < field.Cols; c++)
        output << ", " << field.Plane(0, c);
    output << "]";
    for (int r = 1; r < field.Rows; r++)
    {
        output << ", [" << field.Plane(r, 0);
        for (int c = 1; c < field.Cols; c++)
            output << ", " << field.Plane(r, c);
        output << "]";
    }
    output << "];" << endl;

    // u, prepend a leading zero
    output << "const u = [" << 0;
    for (int i = 0; i < u.size(); i++)
        output << ", " << u[i];
    output << "];" << endl;

    
    // cornerIndexRow
    output << "const cornerIndexRow = [";
    output << "[" << cornerIndexRow.Value(0, 0);
    for (int c = 1; c < cornerIndexRow.getCols(); c++)
        output << ", " << cornerIndexRow.Value(0, c);
    output << "]";
    for (int r = 1; r < cornerIndexRow.getRows(); r++)
    {
        output << ", [" << cornerIndexRow.Value(r, 0);
        for (int c = 1; c < cornerIndexRow.getCols(); c++)
            output << ", " << cornerIndexRow.Value(r, c);
        output << "]";
    }
    output << "];" << endl;
    
    // cornerIndexCol
    output << "const cornerIndexCol = [";
    output << "[" << cornerIndexCol.Value(0, 0);
    for (int c = 1; c < cornerIndexCol.getCols(); c++)
        output << ", " << cornerIndexCol.Value(0, c);
    output << "]";
    for (int r = 1; r < cornerIndexCol.getRows(); r++)
    {
        output << ", [" << cornerIndexCol.Value(r, 0);
        for (int c = 1; c < cornerIndexCol.getCols(); c++)
            output << ", " << cornerIndexCol.Value(r, c);
        output << "]";
    }
    output << "];" << endl;

    // supports
    output << "const supports = {"
            << "SupportRow1: { row: " << supports.SupportRow1.row << ", col: " << supports.SupportRow1.col << "}, "
            << "SupportRow2: { row: " << supports.SupportRow2.row << ", col: " << supports.SupportRow2.col << "}, "
            << "SupportCol: { row: " << supports.SupportCol.row << ", col: " << supports.SupportCol.col << "}}; " << endl;

    // forces
    output << "const forces = [";
    output << "{ attackCorner: { row: " << forces[0].attackCorner.row << ", col: " << forces[0].attackCorner.col << "}, "
            << "forceRow: " << forces[0].forceRow << ", forceCol:" << forces[0].forceCol << "}";
    for (int i = 1; i < forces.size(); i++)
    {
        output << ", { attackCorner: { row: " << forces[0].attackCorner.row << ", col: " << forces[0].attackCorner.col << "}, "
                << "forceRow: " << forces[0].forceRow << ", forceCol:" << forces[0].forceCol << "}";
    }
    output << "]" << endl;
    
    // solutionSteps
    output << "const solutionSteps = " << solutionSteps << ";" << endl;

    // residum
    output << "const residum = " << residum << ";" << endl;

    // stress
    output << "const stress = [" << stress[0];
    for (int i = 1; i < stress.size(); i++)
        output << ", " << stress[i];
    output << "];" << endl;

    // tail
    output << tail;
}

Plotter::~Plotter()
{
}
