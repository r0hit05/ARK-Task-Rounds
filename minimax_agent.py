#!/usr/bin/env python
import sys

import click

from gym_tictactoe.env import TicTacToeEnv, agent_by_mark, check_game_status, after_action_state, tomark, next_mark

def minimax(board,is_max):
    #print("Minimax called")
    curr = check_game_status(board)
    #print(curr)
    if curr == 1:
        #print("max")
        return  10
    if curr == 2:
        #print("min")
        return -10
    if curr == 0:
        #print("draw")
        return 0

    if is_max:
        #print("Maximizer")
        mark = 'O'
        maxval = -100
        for i in range(0,9):
            if board[i]==0:
                old_state = [board,mark]
                new_board,new_mark = after_action_state(old_state,i)
                #print(new_board)
                currval = minimax(new_board,new_mark)
                #print(currval)
                if maxval<currval:
                    maxval = currval
        return maxval
    else:
        #print("minimizer")
        mark = 'X'
        minval=100
        for i in range(0,9):
            if board[i] == 0:
                #print(i)
                old_state = [board,mark]
                new_state = after_action_state(old_state,i)
                #print(new_state[0])
                new_board = new_state[0]
                new_mark = new_state[1]
                currval = minimax(new_board,new_mark)
                #print(currval,mark)
                if minval>currval:
                    minval = currval
        return minval

class HumanAgent(object):
    def __init__(self, mark):
        self.mark = mark

    def act(self, state, ava_actions):
        while True:
            uloc = input("Enter location[1-9], q for quit: ")
            if uloc.lower() == 'q':
                return None
            try:
                action = int(uloc) - 1
                if action not in ava_actions:
                    raise ValueError()
            except ValueError:
                print("Illegal location: '{}'".format(uloc))
            else:
                break

        return action


class MinimaxAgent(object):
    def __init__(self, mark):
        self.mark = mark


#   return the move in this function. ava_actions is an array containting the possible actions
#   you might want to use after_action_state and check_game_status. Also look at env.py
#   state is a tuple with the first value indicating the board and second value indicating mark
#   proper use of inbuilt functions will avoid interacting with state


    def act(self, state, ava_actions):
        opt = -100
        action = -1
        #print(ava_actions)
        for i in ava_actions:
            new_board, mark = after_action_state(state,i)
            #ava_action[i] = 1;
            #print(new_board)
            val = minimax(new_board,False)
            #print(val)
            if val > opt:
                opt = val
                action = i
        return action

@click.command(help="Play minimax agent.")
@click.option('-n', '--show-number', is_flag=True, default=False,
              show_default=True, help="Show location number in the board.")
def play(show_number):
    env = TicTacToeEnv(show_number=show_number)
    agents = [MinimaxAgent('O'),
              HumanAgent('X')]
    episode = 0
    while True:
        state = env.reset()
        _, mark = state
        done = False
        env.render()
        while not done:
            agent = agent_by_mark(agents, mark)
            env.show_turn(True, mark)
            ava_actions = env.available_actions()
            action = agent.act(state, ava_actions)
            if action is None:
                sys.exit()

            state, reward, done, info = env.step(action)

            print('')
            env.render()
            if done:
                env.show_result(True, mark, reward)
                break
            else:
                _, _ = state
            mark = next_mark(mark)

        episode += 1


if __name__ == '__main__':
    # print("Do you want 'O'(first chance) or 'X'? ")
    # p1
    # scanf("%c",p1)
    # if p1.lower() == 'o' or
    play()
