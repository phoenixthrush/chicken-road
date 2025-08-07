#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define CLEAR() system("cls")
#else
#include <unistd.h>
#define CLEAR() system("clear")
#endif

int step_index = 1;
double bet = 5.0;
double balance = 20.0;
double cash = 0.0;
int new_round = 1;
int old_index_for_death = 0;

double get_multiplier(int step)
{
    if (step <= 0)
        return 0;
    return ((int)((1 + 0.15 * step + 0.02 * step * step) * 100 + 0.5)) / 100.0;
}

int get_multiplier_death(int step)
{
    double chance = get_multiplier(step) / 8.0;
    if (chance < 0)
        chance = 0;
    if (chance > 1)
        chance = 1;
    return ((double)rand() / RAND_MAX) < chance;
}

void print_stats()
{
    printf("===============\n");
    printf(" Step     %d\n", step_index);
    printf(" Balance  $%.2f\n", balance);
    printf(" Bet      $%.2f\n", bet);
    printf(" Cash     $%.2f\n", cash);
    printf("===============\n");
}

void print_chicken(int step, int death)
{
    const char *road_pattern[3] = {"\\ / \\ /", " X   X", "/ \\ / \\"};

    const char *chicken_alive[] = {"   \\\\", "   (o>", "\\\\_//)",
                                   " \\_/_)", "  _|_", ""};
    const char *chicken_dead[] = {"   \\\\", "   (x>", "\\\\_xx)",
                                  " \\_x_)", "  _|_", ""};

    const char **art = death ? chicken_dead : chicken_alive;

    for (int i = 0; i < 3; i++)
    {
        for (int s = 1; s <= 10; s++)
        {
            if (s <= step)
                printf("%-7s ", road_pattern[i]);
            else
                printf("%-7s ", "");
        }
        printf("\n");
    }

    printf("\n");

    for (int i = 0; i < 6; i++)
    {
        for (int s = 1; s <= 10; s++)
        {
            int pos = death ? step + 1 : step;
            if (s == pos)
                printf("%-7s ", art[i]);
            else
                printf("%-7s ", "");
        }
        printf("\n");
    }

    for (int s = 1; s <= 10; s++)
    {
        char buf[16];
        snprintf(buf, sizeof(buf), "$%.2f", get_multiplier(s) * bet);
        printf("%-7s ", buf);
    }
    printf("\n");

    for (int s = 0; s < 10; s++)
        printf("%-7s ", "‾‾‾‾‾‾‾");
    printf("\n");
}

void game_loop()
{
    while (1)
    {
        CLEAR();
        printf("A Game By Phoenixthrush\n");
        printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n\n");

        print_stats();

        if (new_round)
        {
            if (balance < bet)
            {
                CLEAR();
                printf("===============\n");
                printf(" NO MONEY LEFT\n");
                printf(" GAME OVER!!!\n");
                printf("===============\n\n");
                print_chicken(old_index_for_death, 1);
                exit(0);
            }
            balance -= bet;
            new_round = 0;
        }

        double money = get_multiplier(step_index) * bet;
        printf("\n");
        print_chicken(step_index, 0);

        if (step_index == 10)
        {
            step_index = 1;
            balance += cash;
            cash = 0;
            new_round = 1;
            continue;
        }

        printf("(J)ump, (C)ashout or (Q)uit? ");
        char input[10];
        fgets(input, sizeof(input), stdin);
        char choice = input[0];

        if (choice == 'J' || choice == 'j')
        {
            if (get_multiplier_death(step_index))
            {
                CLEAR();
                printf("===============\n");
                printf("   YOU DIED\n");
                printf("===============\n");
#ifdef _WIN32
                Sleep(2000);
#else
                sleep(2);
#endif
                old_index_for_death = step_index;
                step_index = 1;
                cash = 0;
                new_round = 1;
                continue;
            }
            step_index++;
            cash = money;
        }
        else if (choice == 'C' || choice == 'c')
        {
            step_index = 1;
            balance += cash;
            cash = 0;
            new_round = 1;
        }
        else if (choice == 'Q' || choice == 'q')
        {
            print_stats();
            exit(0);
        }
    }
}

int main()
{
    srand(time(NULL));
    game_loop();
    return 0;
}
