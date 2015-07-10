#include "ReversiModel.h"

ReversiModel::ReversiModel(Board board)
{
	board_ = board;
	// the initial board configuration
	nonEmptyBoardSquares_.push_back(BoardSquare(4, 4, BLACK));
	nonEmptyBoardSquares_.push_back(BoardSquare(4, 5, WHITE));
	nonEmptyBoardSquares_.push_back(BoardSquare(5, 4, WHITE));
	nonEmptyBoardSquares_.push_back(BoardSquare(5, 5, BLACK));

	// the initial empty boardsquares adjacent to the starting pieces
	adjacentEmptyBoardSquares_.push_back(BoardSquare(3, 3, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(3, 4, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(3, 5, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(3, 6, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(4, 6, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(5, 6, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(6, 6, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(6, 5, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(6, 4, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(6, 3, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(5, 3, EMPTY));
	adjacentEmptyBoardSquares_.push_back(BoardSquare(4, 3, EMPTY));

	Turn* p_turn = new Turn(getValidMoves(board_, BLACK, adjacentEmptyBoardSquares_), BLACK);
	p_turn_ = p_turn;
}

void ReversiModel::setValidMove(int row, int column, Direction* dirs, ValidMove* validMove)
{
	validMove->row = row;
	validMove->column = column;
	validMove->p_dirs = dirs;
}

// use a blob finder algorithm? Should be faster. use a growing vector of possible moves/empty spaces rather than searching the board over and over.


std::vector<BoardSquare> getAdjacentEmptySquares(Board board, int currentSquareRow, int currentSquareColumn)
{
	std::vector<BoardSquare> thing;
	return thing;
	// check all 8 adjacent squares to the current squares and return the empty ones
}


State ReversiModel::getOtherPlayerColour(State playerColour)
{
	if(playerColour == BLACK)
	{
		return WHITE;
	}
	else // playerColour is white
	{
		return BLACK;
	}
}

bool ReversiModel::checkDirection(BoardSquare currentEmptyBoardSquare, Board board, State playerColour, int rowOffset, int columnOffset)
{
	BoardSquare examinedBoardSquare;

	examinedBoardSquare = board.getBoardSquare(currentEmptyBoardSquare.getRow() + rowOffset, currentEmptyBoardSquare.getColumn() + columnOffset);
	// if the board square being examined is of the other player's colour, then there is the potential for a move in this direction
	if (examinedBoardSquare.getState() == getOtherPlayerColour(playerColour))
	{
		// now we check the board square beyond this one to see if it is a valid move 
		examinedBoardSquare = board.getBoardSquare(examinedBoardSquare.getRow() + rowOffset, examinedBoardSquare.getColumn() + columnOffset);
		// if it is of the current player's colour, then this is definitely a valid move
		if (examinedBoardSquare.getState() == playerColour)
		{
			return true;
		}
		// if it is yet another board square of the other player's colour...
		else if (examinedBoardSquare.getState() == getOtherPlayerColour(playerColour))
		{
			// ...we must keep on going until we find a board square that is either of our colour or something else
			while(examinedBoardSquare.getState() == getOtherPlayerColour(playerColour))
			{
				examinedBoardSquare = board.getBoardSquare(examinedBoardSquare.getRow() + rowOffset, examinedBoardSquare.getColumn() + columnOffset);
				// if we found a board square that is of the current player's colour, then this is definitely a valid move
				if (examinedBoardSquare.getState() == playerColour)
				{
					return true;
		        }
				// if we found a board square that is either empty or off board, then we go on to the next direction
				else if (examinedBoardSquare.getState() == EMPTY || OFFBOARD)
				{
					return false;
				}
			}
		}
		// if its not either player colour, then the examined board square is empty or off board, and we must look in a different direction
		else
		{
			 return false;
		}
	}
	// if the board square being examined is not of the other player's colour, then we go on to the next direction
	else
	{
		return false;
	}
}

ValidMove* ReversiModel::getValidMove(BoardSquare currentEmptyBoardSquare, Board board, State playerColour)
{
	// maybe do this better later as well?
	// store the directions for a valid move somewhere while we determine the capture line directions
	int tempDirs[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
	// check left
	if(checkDirection(currentEmptyBoardSquare, board, playerColour, 0, -1))
	{
		tempDirs[0] = LEFT;
	}
	// check up left
	else if (checkDirection(currentEmptyBoardSquare, board, playerColour, -1, -1))
	{
		tempDirs[1] = UP_LEFT;
	}
	// check up
	else if (checkDirection(currentEmptyBoardSquare, board, playerColour, -1, 0))
	{
		tempDirs[2] = UP;
	}
	// check up right
	else if (checkDirection(currentEmptyBoardSquare, board, playerColour, -1, 1))
	{
		tempDirs[3] = UP_RIGHT;
	}
	// check right
	else if (checkDirection(currentEmptyBoardSquare, board, playerColour, 0, 1))
	{
		tempDirs[4] = RIGHT;
	}
	// check down right
	else if (checkDirection(currentEmptyBoardSquare, board, playerColour, 1, 1))
	{
		tempDirs[5] = DOWN_RIGHT;
	}
	// check down
	else if (checkDirection(currentEmptyBoardSquare, board, playerColour, 1, 0))
	{
		tempDirs[6] = DOWN;
	}
	// check down left
	else if (checkDirection(currentEmptyBoardSquare, board, playerColour, 1, -1))
	{
		tempDirs[7] = DOWN_LEFT;
	}
	else
	{
	    return NULL;
	}

	// find the number of directions for possible line captures for the valid move
	int numDirs = 0;
	for (int i = 0; i < 8; i++)
	{
		if (tempDirs[i] != -1)
		{
			numDirs++;
		}
	}
	Direction* dirs = new Direction[numDirs];
	// also, code this better? it looks weird.
	// assign the line captures to dir
	int j = 0;
	for (int i = 0; i < 8; i++)
	{
		if (tempDirs[i] != -1)
		{
			dirs[j] = (Direction)tempDirs[i];
			j++;
		}
	}
	ValidMove* p_validMove = new ValidMove;
	setValidMove(currentEmptyBoardSquare.getRow(), currentEmptyBoardSquare.getColumn(), dirs, p_validMove);
	return p_validMove;
}

ValidMove* ReversiModel::getValidMoves(Board board, State playerColour, std::vector<BoardSquare> adjacentEmptyBoardSquares)
{
	BoardSquare currentEmptyBoardSquare;
	std::stack<ValidMove*> tempValidMoveStack;
	ValidMove* p_tempValidMove = new ValidMove;
	int tempValidMoveStackSize;
	ValidMove* p_ValidMoves;

	while (!adjacentEmptyBoardSquares.empty())
	{
	    currentEmptyBoardSquare = adjacentEmptyBoardSquares.back();
		p_tempValidMove = getValidMove(currentEmptyBoardSquare, board, playerColour);
		if (p_tempValidMove != NULL)
		{
			tempValidMoveStack.push(p_tempValidMove);
		}
		adjacentEmptyBoardSquares.pop_back();
	}
	// hope this doesn't break anything
	delete p_tempValidMove;

	tempValidMoveStackSize = tempValidMoveStack.size();

	p_ValidMoves = new ValidMove[tempValidMoveStackSize];	
	while (!tempValidMoveStack.empty())
	{
		p_ValidMoves = tempValidMoveStack.top();
		p_ValidMoves++;
		tempValidMoveStack.pop();
	}
	// return p_ValidMoves back to beginning
	for (int i = tempValidMoveStackSize; i > 0; i--)
	{
		p_ValidMoves--;
	}
	return p_ValidMoves;
}

// don't forget to change getBoardSquare to verify if it returns an offBoard boardsquare.
void ReversiModel::resolveMove(Board &board, BoardSquare chosenBoardSquare, Turn* p_turn)
{
	//******************************* for debugging purposes; create function checkMove
	ValidMove* p_validMoves = p_turn->getValidMoves();
	ValidMove chosenValidMove;
	Direction* p_dirs;
	BoardSquare currentBoardSquare = chosenBoardSquare;
	int chosenBoardSquareRow = chosenBoardSquare.getRow();
	int chosenBoardSquareColumn = chosenBoardSquare.getColumn();
	State playerColour;
	
	while (p_validMoves != NULL)
	{
		if (chosenBoardSquareRow == p_validMoves->row && chosenBoardSquareColumn == p_validMoves->column)
		{
			chosenValidMove = *p_validMoves;
			break;
		}
		p_validMoves++;
	}
	//*************************

	p_dirs = chosenValidMove.p_dirs;
	for (int i = 0; p_dirs[i] != NULL; i++)
	{
		switch (p_dirs[i])
		{
		    case LEFT:	
                flipBoardSquares(currentBoardSquare, chosenBoardSquare, p_turn->getPlayerColour(), board, 0, -1);
				break;
			case UP_LEFT:
				flipBoardSquares(currentBoardSquare, chosenBoardSquare, p_turn->getPlayerColour(), board, -1, -1);
				break;
			case UP:
				flipBoardSquares(currentBoardSquare, chosenBoardSquare, p_turn->getPlayerColour(), board, -1, 0);
				break;
			case UP_RIGHT:
				flipBoardSquares(currentBoardSquare, chosenBoardSquare, p_turn->getPlayerColour(), board, -1, 1);
				break;
			case RIGHT:
				flipBoardSquares(currentBoardSquare, chosenBoardSquare, p_turn->getPlayerColour(), board, 0, 1);
				break;
			case DOWN_RIGHT:
				flipBoardSquares(currentBoardSquare, chosenBoardSquare, p_turn->getPlayerColour(), board, 1, 1);
				break;
			case DOWN:
				flipBoardSquares(currentBoardSquare, chosenBoardSquare, p_turn->getPlayerColour(), board, 1, 0);
				break;
			case DOWN_LEFT:
				flipBoardSquares(currentBoardSquare, chosenBoardSquare, p_turn->getPlayerColour(), board, 1, -1);
				break;
		}
	}

	// now, update the View and delete the turn because it is finished.
	updateView();

	playerColour = p_turn->getPlayerColour();
	// look up how destructors work
	delete p_turn;

	// might delegate some of this functionality to another function and make some of these functions private too
	p_turn = new Turn(getValidMoves(board, getOtherPlayerColour(playerColour), adjacentEmptyBoardSquares_), getOtherPlayerColour(playerColour));
}

void ReversiModel::flipBoardSquares(BoardSquare currentBoardSquare, BoardSquare chosenBoardSquare, State playerColour, Board &board, int rowOffset, int columnOffset)
{
	while (currentBoardSquare.getState() != playerColour)
	{
		// here we finally alter the board.
		board.setBoardSquareState(currentBoardSquare, playerColour); 
		currentBoardSquare = board.getBoardSquare(currentBoardSquare.getRow() + rowOffset, currentBoardSquare.getColumn() + columnOffset);
	}
	// set it back for the next loop
	currentBoardSquare = chosenBoardSquare;
}

void ReversiModel::updateView(){}