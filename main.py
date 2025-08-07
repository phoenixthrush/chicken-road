import os
import random
import time


def clear():
    os.system("cls" if os.name == "nt" else "clear")


def get_multiplier(step):
    if step <= 0:
        return 0
    return round(1 + 0.15 * step + 0.02 * step**2, 2)


def get_multiplier_death(step):
    chance = get_multiplier(step) / 9.0
    chance = max(0, chance)
    return random.random() < chance


index = 1
bet = 5.0
balance = 20.0
cash = 0
new_round = True
old_index_for_death = 0


def print_stats():
    print("=" * 15)
    print(f" Step     {index}")
    print(f" Balance  ${round(balance, 2)}")
    print(f" Bet      ${bet}")
    print(f" Cash     ${cash}")
    print("=" * 15)


def print_chicken(step, death=False):
    road_pattern = ["\\ / \\ /", " X   X", "/ \\ / \\"]

    chicken_art = (
        ["   \\\\", "   (o>", "\\\\_//)", " \\_/_)", "  _|_", ""]
        if not death
        else ["   \\\\", "   (x>", "\\\\_xx)", " \\_x_)", "  _|_", ""]
    )

    for line_index in range(len(road_pattern)):
        for current_step in range(1, 11):
            if current_step <= step:
                print(road_pattern[line_index].ljust(8), end="")
            else:
                print(" " * 8, end="")
        print()

    print()

    for line_index in range(len(chicken_art)):
        for current_step in range(1, 11):
            chicken_position = step + 1 if death else step
            if current_step == chicken_position:
                print(chicken_art[line_index].ljust(8), end="")
            else:
                print(" " * 8, end="")
        print()

    for current_step in range(1, 11):
        price = f"${round(get_multiplier(current_step) * bet, 2)}"
        print(price.ljust(8), end="")
    print()

    for current_step in range(1, 11):
        print("‾" * 7 + " ", end="")
    print()


def game_loop():
    global index, cash, balance, bet, new_round, old_index_for_death

    clear()
    print(
        """A Game By Phoenixthrush
‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾"""
    )
    print()
    print_stats()

    if new_round:
        if round(balance, 2) < bet:
            clear()
            print("=" * 15)
            print(" NO MONEY LEFT")
            print(" GAME OVER!!!")
            print("=" * 15)
            print()
            print_chicken(old_index_for_death, True)
            exit()
        balance -= bet
        new_round = False

    money = round(get_multiplier(index) * bet, 2)
    print()
    print_chicken(index)

    if index == 10:
        index = 1
        balance += cash
        cash = 0
        new_round = True
        game_loop()
    choice = input("(J)ump, (C)ashout or (Q)uit? ")
    match choice.upper():
        case "J":
            if get_multiplier_death(step=index):
                clear()
                print("=" * 15)
                print("   YOU DIED")
                print("=" * 15)
                time.sleep(2)
                old_index_for_death = index
                index = 1
                cash = 0
                new_round = True
                game_loop()
            index += 1
            cash = money
            game_loop()

        case "C":
            index = 1
            balance += cash
            cash = 0
            new_round = True
            game_loop()

        case "Q":
            print_stats()
            exit()

        case _:
            game_loop()


try:
    game_loop()
except KeyboardInterrupt:
    print("\nGame interrupted. Goodbye!")
    exit()
