#include <stdio.h>
#include "../lib/raylib/src/raylib.h"
#include <stdlib.h>
#include <time.h>

#define SOLID_COLOR(r,g,b) (Color) { r, g, b, 255}

#define BG_COLOR SOLID_COLOR(33,158,188)
#define X_COLOR SOLID_COLOR(251, 133, 0)
#define O_COLOR SOLID_COLOR(255, 183, 3)
#define GRID_COLOR SOLID_COLOR(2, 48, 71)

#define TRANSPARENCY 20

#define BG_ARTIFACTS_COLOR SOLID_COLOR(142, 202, 230)

#define PADDING 10
#define GRID_LINE_WIDTH 5
#define PIECE_THICKNESS 10

#define TIPS_FADEOUT_TIME 3
#define FONTSIZE 40
#define FONTCOLOR SOLID_COLOR(2, 7, 93)

typedef struct {
	int x;
	int y;
} intVector2;

typedef struct {
	intVector2* data;
	int size;
} intVector2Arr;

typedef enum {
	Empty,
	X,
	O
} Cell;

typedef struct {
	Cell** cells;
	int size;
} Table;

Table* initTable() {
	Table* table = malloc(sizeof(Table));
	table->size = 3;
			
	table->cells = malloc(sizeof(Cell*) * table->size);

	
	for(int i = 0; i < table->size; i++) {
		table->cells[i] = malloc(sizeof(Cell*) * table->size);
		for(int j = 0; j < table->size; j++) {
			table->cells[i][j] = Empty;
		}
	}

	
	return table;
}

void freeTable(Table* table) {
	if(table == NULL) return;

	if(table->cells == NULL) {
		free(table);
		table = NULL;
		return;
	}
	
	for(int i = 0; i < table->size; i++) {
		free(table->cells[i]);
	}

	free(table->cells);	
	
	free(table);	
	table = NULL;
}

Rectangle* recalcRectsFromTable(Rectangle* rects, Table* table, Vector2* pos, Vector2* tableSizePx) {
	float cellWidth = tableSizePx->x / table->size;
	float cellHeight = tableSizePx->y / table->size;

	
	int length = table->size * table->size;

	rects = realloc(rects, sizeof(Rectangle) * length); 
	
	int index = 0;
	for(int i = 0; i < table->size; i++) {
		for(int j = 0; j < table->size; j++) {
		
			rects[index] = (Rectangle){ pos->x + (j * cellWidth), pos->y + (i * cellHeight), cellWidth, cellHeight};
			 
			index++;
		}
	}
	
	return rects;
}

Rectangle* rectsFromTable(Table* table, Vector2* pos, Vector2* tableSizePx) {
	return recalcRectsFromTable(NULL, table, pos, tableSizePx);
}

void drawXColor(Rectangle* cell, Color* color) {
	{
		int x = cell->x + PADDING;
		int y = cell->y + PADDING;
		int endX = cell->x + cell->width - PADDING;
		int endY = cell->y + cell->height - PADDING;
		
		Vector2 start = {x, y};
		Vector2 end = {endX, endY};
		
		DrawLineEx(start, end, PIECE_THICKNESS, *color);
	}

	{
		int x = cell->x + cell->width - PADDING;
		int y = cell->y + PADDING;
		int endX = cell->x + PADDING;
		int endY = cell->y + cell->height - PADDING;

		Vector2 start = {x, y};
		Vector2 end = {endX, endY};
		
		DrawLineEx(start, end, PIECE_THICKNESS, *color);
	}

}


void drawXTrans(Rectangle* cell) {
	Color color = X_COLOR;
	color.a = TRANSPARENCY;
	drawXColor(cell, &color);
}

void drawX(Rectangle* cell) {
	Color color = X_COLOR;
	drawXColor(cell, &color);
}

void drawOColor(Rectangle* cell, Color* color) {
	int x = cell->x + cell->width  / 2;
	int y = cell->y + cell->height / 2;
	float rh = (cell->width / 2) - PADDING;
	float rv = (cell->height / 2) - PADDING;

	for(int i = 0; i < PIECE_THICKNESS * 2; i++) {
		float d = (float)i / 2;
		DrawEllipseLines(x,y,rh - d,rv - d, *color);
	}
}

void drawOTrans(Rectangle* cell) {
	Color color = O_COLOR;
	color.a = TRANSPARENCY;
	drawOColor(cell, &color);
}

void drawO(Rectangle* cell) {
	Color color = O_COLOR;
	drawOColor(cell, &color);
}

void drawTable(Table* table, Rectangle* rects) {
	
	int index = 0;
	for(int i = 0; i < table->size; i++) {
		for(int j = 0; j < table->size; j++) {
			Rectangle* cell = &rects[index];
			
			if(i != table->size && j != table->size-1) {
				int x = cell->x + cell->width;
				int y = cell->y;

				int endX = x;
				int endY = cell->y + cell->height;
				
				Vector2 start = {x, y};
				Vector2 end = {endX, endY};
				
				DrawLineEx(start, end, GRID_LINE_WIDTH, GRID_COLOR);
			}

			if(i != table->size-1) {
				int x = cell->x;
				int y = cell->y + cell->height;

				int endX = cell->x + cell->width;
				int endY = cell->y + cell->height;

				
 				Vector2 start = {x, y};
 				Vector2 end = {endX, endY};
 				
				DrawLineEx(start, end, GRID_LINE_WIDTH, GRID_COLOR);
			}

			if(table->cells[i][j] == X) drawX(cell);
			if(table->cells[i][j] == O) drawO(cell);
			
			index++;
		}
	} 
}



Vector2 calcTableSizePx(int width, int height, int tablePadding) {
	int paddAll= tablePadding * 2;
	return (Vector2){ width - paddAll, height - paddAll};
}

bool onCellClick(Table* table, Rectangle* rects, Vector2* mousePos, Cell player) {

	int rectIndex = 0;
	bool collisionDetected = false;
	bool validMove = false;
	
	for(int i = 0; i < table->size && !collisionDetected; i++) {
		for(int j = 0; j < table->size; j++) {
			if(!CheckCollisionPointRec(*mousePos, rects[rectIndex])) {
				rectIndex++;
				continue;
			}

			
			
			if(table->cells[i][j] == Empty) {
				table->cells[i][j] = player;
				validMove = true;
			}
			
			collisionDetected = true;
			break;
		}
	}
	
	return validMove;
}

void onCellHover(Table* table, Rectangle* rects, Vector2* mousePos, Cell player) {
	int rectIndex = 0;

	for(int i = 0; i < table->size; i++) {
		for(int j = 0; j < table->size; j++) {
			if(!CheckCollisionPointRec(*mousePos, rects[rectIndex]) || !(table->cells[i][j] == Empty)) {
				rectIndex++;
				continue;
			} 
  			if(table->cells[i][j] == Empty) player == X ? drawXTrans(&rects[rectIndex]) : drawOTrans(&rects[rectIndex]);
			
		}
	}
}

bool isTableEmpty(Table* table) {
	 
	for(int i = 0; i < table->size; i++) {
		 
		for(int j = 0; j < table->size; j++) {
			if(table->cells[i][j] != Empty) return false;
		}
	}
	return true;
}


bool isTableFull(Table* table) {
	 
	for(int i = 0; i < table->size; i++) {
		 
		for(int j = 0; j < table->size; j++) {
			if(table->cells[i][j] == Empty) return false;
		}
	}
	return true;
}

Vector2 getTextCenter(char* text) {
	int width = MeasureText(text, FONTSIZE);
	
	return (Vector2){ width / 2, FONTSIZE / 2 };
}

void endingScreen(Cell player, Cell winner, Vector2* center) {
	char* endingText = "You Win!";
	Color color = GREEN;
	
	if(player != winner) { 
		endingText = "You Lose!";
		color = RED;
	}
	
	if(winner == Empty) {
		endingText = "Tie!";
		color = BLUE;
	}
	
	Vector2 textCenter = getTextCenter(endingText);
	int x = center->x - textCenter.x;
	int y = center->y - textCenter.y;
	
	DrawText(endingText, x, y, FONTSIZE, color);
}

void clearTable(Table* table) {
	for(int i = 0; i < table->size; i++) {
		for(int j = 0; j < table->size; j++) {
			table->cells[i][j] = Empty;
		}
	}
}

bool didPieceWin(Table* table, Cell piece) {
	bool result = true;
	
	// horizontal check
	for(int i = 0; i < table->size; i++) {
		result = true;
		for(int j = 0; j < table->size; j++) {
			if(table->cells[i][j] != piece) {
				result = false;
			} 
		}
		if(result) return result;
	}
 	
	// vertical check
	for(int j = 0; j < table->size; j++) {
		result = true;
		for(int i = 0; i < table->size; i++) {
			if(table->cells[i][j] != piece) {
				result = false;
			}
		}
		if(result) return result;
	}

	result = true;
	// top left to right down diagonal check
	for(int i = 0; i < table->size; i++) {
		if(table->cells[i][i] != piece) result = false;
	}
	
	if(result) return result;
	
	result = true;
	// top right to left down diagonal check
	for(int i = 0; i < table->size; i++) {
		if(table->cells[i][table->size - (i + 1)] != piece) result = false;
	}
	
	return result;
}

Cell determineWinner(Table* table) {
	if(didPieceWin(table, X)) return X;
	if(didPieceWin(table, O)) return O;
				
	return Empty;
}

intVector2 getPerfectMove(intVector2Arr* possibleMoves){
	srand(time(0));
	return  possibleMoves->data[(rand() % possibleMoves->size)];
}

intVector2Arr gatherAllPossibleMoves(Table* table) {
	intVector2Arr result = {};
	result.data = malloc(sizeof(intVector2) * (table->size * table->size));
	
	 
	for(int i = 0; i < table->size; i++) { 
		for(int j = 0; j < table->size; j++) {
			if(table->cells[i][j] == Empty) {
				result.data[result.size] = (intVector2){i,j};
				result.size++;
			}
		}
					
	}

	return result;
}
 
void determineOpponentMove(Table* table, Cell opponent) {
	

	int c = table->size / 2;
	if(table->cells[c][c] == Empty){ 
		table->cells[c][c] = opponent;
		return;
	}

	
	intVector2Arr possibleMoves = gatherAllPossibleMoves(table);
	intVector2 perfectMove = getPerfectMove(&possibleMoves);
		 
 	Cell* cell = &table->cells[perfectMove.x][perfectMove.y];
 	*cell = opponent;
 	
	free(possibleMoves.data);
 	possibleMoves.data = NULL;
 	possibleMoves.size = 0;
 	
	return;
}

void restart(Table* table,Cell* winner, float* tipsTime, bool* showTips,  Cell* lastPlayed) {
	
	
	clearTable(table);
	
	(void)(winner);
	*winner = Empty;

	(void)(showTips);
	*showTips = true;
	
	(void)(tipsTime);
	*tipsTime = 0;

	(void)(lastPlayed);
	*lastPlayed = O;
}

int main(void) {

	Cell player = X;
	Cell opponent = player == X ? O : X;
	Cell winner = Empty;
	bool ended = false;
	Cell lastPlayed = O;
	
	Table* table = initTable();
 	int width = 800; 
	int height = 800; 
	Vector2 center = {width / 2, height / 2};
 	
 	const char *title = "Tik Tak Toe";
 	
 	InitWindow(width, height, title);
 	SetWindowState(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
 	
 	float tablePadding = 10;
 	
 	Vector2 tableSizePx = calcTableSizePx(width, height, tablePadding);
 	 
 
 	Vector2 tablePos = { tablePadding, tablePadding};
 	Rectangle* rects = rectsFromTable(table,&tablePos, &tableSizePx);

 	float tipsTime = 0;
 	bool showTips = true;
 	
 	char* tip1Text = "Press ESC to Quit.";
	Vector2 tip1TextCenter = getTextCenter(tip1Text);
	
 	char* tip2Text = "Click a cell to play your move";
	Vector2 tip2TextCenter = getTextCenter(tip2Text);
	
	char* tip3Text = "Press R to restart";
	Vector2 tip3TextCenter = getTextCenter(tip3Text);
 
	
 	while(!WindowShouldClose()) {
  
 		
 		if(showTips) tipsTime += GetFrameTime();
 		
		if(IsWindowResized()) {
			width = GetScreenWidth();
			height = GetScreenHeight();
			center = (Vector2){width / 2, height / 2};
			
			tableSizePx = calcTableSizePx(width, height, tablePadding);
			recalcRectsFromTable(rects, table, &tablePos, &tableSizePx);
		}
		
		if(GetKeyPressed() == KEY_R) restart(table, &winner, &tipsTime, &showTips , &lastPlayed);

		Vector2 mousePos = GetMousePosition();
		
		bool mouseClickUsed = false;
		bool mouseClick = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
		
		if(lastPlayed == opponent) {
			if(mouseClick && !ended && !showTips) {
   
				bool validMove = onCellClick(table, rects, &mousePos, player);
				if(validMove) {
					lastPlayed = player;
					mouseClickUsed = true;
				}
				
			}

		} else {
			if (!showTips && !ended) {

				determineOpponentMove(table, opponent);
				lastPlayed = opponent;
 
			}
		}
		
		winner = determineWinner(table);

		ended = winner != Empty || isTableFull(table);		

		if(mouseClick && !mouseClickUsed) {
			
			if(ended) {				
			 	restart(table, &winner, &tipsTime, &showTips , &lastPlayed);
			} else showTips = false;

			
			mouseClickUsed = true;
		}
		
 		BeginDrawing();
 		ClearBackground(BG_COLOR);
		
 		drawTable(table, rects);
 		
		if(!ended && !showTips) onCellHover(table, rects, &mousePos, player);
		
 		if(showTips){ 
 			showTips = (tipsTime < TIPS_FADEOUT_TIME) && isTableEmpty(table);
 			int tip1X = center.x - tip1TextCenter.x;
 			int tip1Y = center.y - tip1TextCenter.y;
 			
 			DrawText(tip1Text, tip1X, tip1Y, FONTSIZE, FONTCOLOR);

 			int tip2X = center.x - tip2TextCenter.x;
 			int tip2Y = tip1Y + FONTSIZE;
 				 				
 			DrawText(tip2Text, tip2X, tip2Y, FONTSIZE, FONTCOLOR);


			int tip3X = center.x - tip3TextCenter.x;
 			int tip3Y = tip2Y + FONTSIZE;
 				 				
 			DrawText(tip3Text, tip3X, tip3Y, FONTSIZE, FONTCOLOR);
 	
 		}

		if(ended) {
			endingScreen(player, winner, &center);
		}
 		EndDrawing();
 	}	
 	
	CloseWindow();

	freeTable(table);
	free(rects);
	
	return 0;
}
