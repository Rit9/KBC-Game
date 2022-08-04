#include <stdio.h>
#include <stdlib.h>
#include <time.h> //To be used for seeding the random number generating function.

void instructions();
int check_solution(char, int, int);
void lifeline1(int, int *);
void randomizer(int *);
char get_user_input();
void enter_to_cont();
char *questions(int);
void clear_stdin();

int main(void)
{
    int flag_sol = 0, flag_l1 = 1, flag_l2 = 1, q_order[18], q_ctr = 0, l2_easy = 0, l2_med = 0;
    //flag_l1 = 1 means lifeline1 is available; similarly for lifeline2.
    //l2_easy = 0 means lifeline2 is NOT used in the easy segment; similar thing applies to l2_med.

    char user_in, *user_prize = "Rs. 00";
    char *prize[15] = {"Rs. 1,000", "Rs. 2,000", "Rs. 5,000", "Rs. 10,000", "Rs. 50,000", "Rs. 90,000", "Rs. 2,00,000", "Rs. 3,20,000", "Rs. 6,00,000", "Rs. 12,00,000", "Rs. 25,00,000", "Rs. 50,00,000", "Rs. 1,00,00,000", "Rs. 5,00,00,000", "Rs. 7,00,00,000"};

    //To show the questions in random order (within each difficulty segment) every time the game is played...
    randomizer(q_order);
    /*After execution of randomizer func, q_order[18] array will be randomized with integers 0-3 in random order on the
    first four places, integers 4-9 in random order on the next six places and integers 10-17 in random order on the
    last eight places. */

    instructions(); //To display the information and instructions regarding the game.
    printf("\n");

    printf("*LET THE GAME BEGIN!*\n");
    for (int j = 0; j < 15; j++, q_ctr++)
    {
        printf("------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Your current prize money is: %s\n", user_prize);
        printf("Last intermediate level crossed: ");
        if (j < 3)
            printf("NONE\n");
        else if (j > 2 && j < 8)
            printf("LEVEL 1 - Rs. 5,000 (Q3)\n");
        else if (j > 7 && j < 12)
            printf("LEVEL 2 - Rs. 3,20,000 (Q8)\n");
        else
            printf("LEVEL 3 - Rs. 50,00,000 (Q12)\n");
        printf("*Prize money with respect to this question: %s*\n", prize[j]);

        printf("\nQ.%d: %s", j + 1, questions(q_order[q_ctr]));
        /*q_ctr is used for moving to the next element of the q_order[18] array with each successive iteration of the for
      loop. Also, value of the (q_ctr)th element of the array will be passed to the function having the question bank
      so as to fetch a question corresponding to that element.*/
        printf("\nNOTE:");
        if (flag_l1 == 1)
            printf(" Enter 'L' for lifeline1 (50-50).");
        if (flag_l2 == 1)
            printf(" Enter 'l' for lifeline2 (Flip the question).");
        printf(" Enter 'q' to quit the game.\n");
        printf("IMPORTANT: Only valid input will be accepted (i.e. option A, B, C or D *in Upper Case* or special option(s) as mentioned in the note above).\n");
        user_in = get_user_input(); //for fetching the user response and storing it in a var.

        //Lifelines can be used in a 'nested' way...

        if (user_in == 'L') //User wants to use lifeline1
        {
            lifeline1(q_order[q_ctr], &flag_l1);

            //To ensure that the user-input is a valid one...
            while (1)
            {
                user_in = get_user_input();
                if (user_in == 'l' || user_in == 'q' || user_in == 'A' || user_in == 'B' || user_in == 'C' || user_in == 'D')
                    break;
            }
        }

        if (user_in == 'l') //User wants to use lifeline2
        {
            if (flag_l2 == 0)
            {
                printf("\nSorry, you can't use lifeline2 again as you've already used it.\n");

                //To ensure that the user-input is a valid one...
                while (1)
                {
                    user_in = get_user_input();
                    if (user_in == 'L' || user_in == 'q' || user_in == 'A' || user_in == 'B' || user_in == 'C' || user_in == 'D')
                        break;
                }

                if (user_in == 'L') //User wants to use lifeline1
                {
                    lifeline1(q_order[q_ctr], &flag_l1);

                    //To ensure that the user-input is a valid one...
                    while (1)
                    {
                        user_in = get_user_input();
                        if (user_in == 'q' || user_in == 'A' || user_in == 'B' || user_in == 'C' || user_in == 'D')
                            break;
                    }
                }
            }
            else
            {
                printf("\nYou've opted for lifeline2 (Flip the question). So your question is changed (difficulty level of the question remains the same).\n");
                flag_l2 = 0; //To avoid re-use of lifeline2
                //To keep track of the segment in which lifeline2 is being used...
                if (q_ctr < 3)
                    l2_easy = 1; //lifeline2 is being used in easy segment
                else if (q_ctr < 9)
                    l2_med = 1; //lifeline2 is being used in medium segment
                j--;
                continue;
            }
        }

        if (user_in == 'q') //In case the user wants to quit the game
        {
            printf("************************************************************************************************************************************************************\n");
            printf("Ok, so you've decided to quit. THE PRIZE MONEY THAT YOU HAVE WON IS %s\n", user_prize);
            printf("Thank you for playing :-)\n");
            printf("************************************************************************************************************************************************************\n");
            enter_to_cont(); //To ensure that the executable doesn't close abruptly.
            return 0;
        }

        flag_sol = check_solution(user_in, q_order[q_ctr], j);
        if (flag_sol == 1)
        {
            printf("You have won %s\n", prize[j]);
            user_prize = prize[j];
        }
        else
        {
            enter_to_cont(); //To ensure that the executable doesn't close abruptly.
            return 0;
        }

        enter_to_cont();

        //To skip the extra 'easy level' question if lifeline2 is not used in easy segment
        if (l2_easy == 0)
        {
            if (q_ctr == 2)
                q_ctr++;
        }
        //To skip the extra 'medium level' question if lifeline2 is not used in medium segment
        if (l2_med == 0)
        {
            if (q_ctr == 8)
                q_ctr++;
        }
    }

    //If the user answers all the 15 questions correctly...
    printf("************************************************************************************************************************************************************\n");
    printf("CONGRATULATIONS! YOU HAVE COMPLETED THE GAME AND WON %s\n", user_prize);
    printf("Thank you for playing :-)\n");
    printf("************************************************************************************************************************************************************\n");

    enter_to_cont(); //To ensure that the executable doesn't close abruptly.
    return 0;
}

//All of the instructions and info regarding the game is contained in this - to be displayed only once(in the starting).
void instructions()
{
    printf("              WELCOME TO KAUN BANEGA CROREPATI (KBC)              \n");
    printf(" \n");
    printf("   The instructions to the game are as follows:\n");
    printf("\n");
    printf("1. Please play the game in full screen.\n");
    printf("2. This game consists a total of 15 questions.\n");
    printf("   All the questions are of multiple choice in which you have to select one correct option out of the four choices given and type that option (Upper Case A or B or C or D)    in the space provided.\n");
    printf("   Make sure you've chosen the option that you really wanted. Once sure, press the enter key to submit the option.\n");
    printf("\n");
    printf("3. Prize money with respect to each question are as follows:\n");
    printf("   1 - Rs. 1,000 \n   2 - Rs. 2,000\n   *3 - Rs. 5,000*\n   4 - Rs. 10,000\n   5 - Rs. 50,000\n   6 - Rs. 90,000\n");
    printf("   7 - Rs. 2,00,000\n   *8 - Rs. 3,20,000*\n   9 - Rs. 6,00,000\n   10 - Rs. 12,00,000\n   11 - Rs. 25,00,000\n");
    printf("   *12 - Rs. 50,00,000*\n   13 - Rs. 1,00,00,000\n   14 - Rs. 5,00,00,000\n   15 - Rs. 7,00,00,000\n");
    printf("   i.e. your prize money will become Rs. 1,000 if you answer Q1 correctly, it'll become Rs. 2,000 if you answer Q2 correctly and so on.\n");
    printf("\n");
    printf("4. There are two lifelines which can be used if you get stuck in answering any question. However these can be used only once. \n ");
    printf("\n");
    printf("   Lifeline 1: 50-50              (This lifeline omits 2 wrong options from the 4 options given and leave you with only 2 options to answer from.)\n");
    printf("   Lifeline 2: Flip the question  (This lifeline replaces the current question with a new one of the same difficulty level.)\n");
    printf("   To use Lifeline 1, enter the option as 'L'. Similarly for Lifeline 2, enter the option as 'l'.\n");
    printf("\n");
    printf("5. Category of questions on the basis of difficulty level are as:\n");
    printf("   1 - 3: Easy segment\n   4 - 8: Medium segment\n   9 - 15: Difficult segment\n");
    printf("\n");
    printf("6. Intermediate levels of the game are set at question numbers 3, 8 and 12.\n");
    printf("   If you successfully cross an intermediate level and answer a question wrongly after that, your prize money will be reduced to that intermediate level only and not to 0.\n ");
    printf("\n");
    printf("7. You can quit the game anytime you want by entering the option as 'q' instead of A, B, C or D.\n");
    printf("8. Keep in mind that if you input more than one characters, only the first character will be considered and rest will be ignored.\n");

    enter_to_cont();
}

//This function will accomplish 2 tasks -  prompt the user to give a response and ensure that the input is valid.
char get_user_input()
{
    char return_char;

    while (1)
    {
        printf("---> Choose your option: ");
        scanf(" %c", &return_char);
        if (return_char == 'L' || return_char == 'l' || return_char == 'q' || return_char == 'A' || return_char == 'B' || return_char == 'C' || return_char == 'D')
            break;
        clear_stdin(); //for clearing the input buffer
    }
    clear_stdin(); //for clearing the input buffer
    return return_char;
}

int check_solution(char input, int q_id, int q_no)
{
    //correct options corresponding to each question of the question bank...
    char sol[] = {'B', 'A', 'D', 'C', 'C', 'B', 'D', 'B', 'C', 'A', 'C', 'B', 'D', 'B', 'C', 'A', 'A', 'D'};

    if (input == sol[q_id])
    {
        printf("\nGREAT! You have given correct answer.\n");
        return 1;
    }
    else
    {
        printf("\nOH NO! You have given incorrect answer. The correct option was %c.\n", sol[q_id]);
        printf("************************************************************************************************************************************************************\n");
        printf("THE PRIZE MONEY THAT YOU HAVE WON IS ");
        if (q_no < 3)
            printf("Rs. 00\n");
        else if (q_no > 2 && q_no < 8)
            printf("Rs. 5,000\n");
        else if (q_no > 7 && q_no < 12)
            printf("Rs. 3,20,000\n");
        else
            printf("Rs. 50,00,000");
        printf("Thank you for playing :-)\n");
        printf("************************************************************************************************************************************************************\n");
        return 0;
    }
}

void lifeline1(int q_id, int *flag_lifeline1)
{
    if (*flag_lifeline1 == 1)
    {
        printf("\nYou've opted for Lifeline1 (50-50). Hence 2 wrong options are eliminated. Select your option from the 2 given below...\n");
        printf("WARNING: If you submit an option from the 2 eliminated ones, it will be recognized as incorrect and you'll lose.\n");

        switch (q_id)
        {
        case 0:
            printf(" B. September 8\n D. June 30\n");
            break;
        case 1:
            printf(" A. Platypus\n B. Blue Whale\n");
            break;
        case 2:
            printf(" A. Pakistan\n D. Bangladesh\n");
            break;
        case 3:
            printf(" A. Table Tennis\n C. Lawn Tennis\n");
            break;
        case 4:
            printf(" B. Alfred Nobel\n C. Thomas Edison\n");
            break;
        case 5:
            printf(" A. Dignity for all - focus on women\n B. Dignity for all - focus on Children\n");
            break;
        case 6:
            printf(" B. Prem Chand\n D. Amrit Lal Nagar\n");
            break;
        case 7:
            printf(" A. Dance & Drama\n B. Fine Arts\n");
            break;
        case 8:
            printf(" B. Hubble Space Telescope\n C. Event Horizon Telescope\n");
            break;
        case 9:
            printf(" A. 1983\n D. 1984\n");
            break;
        case 10:
            printf(" B. Commandur Rangachari\n C. Gogumal Kishenchand\n");
            break;
        case 11:
            printf(" A. Solicitor General\n B. Attorney General\n");
            break;
        case 12:
            printf(" B. Fort Naarden\n D. Fort Chambray\n");
            break;
        case 13:
            printf(" A. Junko Tabei\n B. Wanda Rutkiewicz\n");
            break;
        case 14:
            printf(" B. Parvati\n C. Saraswati\n");
            break;
        case 15:
            printf(" A. Moth\n D. Japanese beetle\n");
            break;
        case 16:
            printf(" A. Lesotho\n B. Burkina Faso\n");
            break;
        default:
            printf(" B. Elmer Sperry\n D. Igor Sikorsky\n");
        }

        *flag_lifeline1 = 0; //To avoid reuse of lifeline 1
    }
    else
    {
        printf("\nSorry, you can't use lifeline1 again as you've already used it.\n");
    }
}

//Function which will randomize the question order within each segment (i.e. Easy, Medium & Difficult questions)
/*After execution of this func, q_order[18] array will be randomized with integers 0-3 in random order on the
first four places, integers 4-9 in random order on the next six places and integers 10-17 in random order on the
last eight places. */
void randomizer(int q_order[])
{
    int temp = 0, repeat;
    srand(time(NULL));
    /*srand() func is used for seeding the rand() func. The argument of srand() should be different everytime the
    program is executed to ensure that q_order[18] doesn't look exactly the same everytime... hence time(NULL) is being used.*/

    //To ensure that first 4 places of q_order[18] array are filled with integers 0-3 in random fashion, without repetition.
    q_order[0] = (rand() % 4);
    for (int i = 1; i <= 3; i++)
    {
        repeat = 1;
        while (repeat == 1)
        {
            temp = (rand() % 4);
            for (int j = 0; j < i; j++)
            {
                if (temp == q_order[j])
                {
                    repeat = 1;
                    break;
                }
                else
                    repeat = 0;
            }
        }
        q_order[i] = temp;
    }

    //To ensure that next 6 places (after the first 4) of q_order[18] array are filled with integers 4-9 in random fashion, without repetition.
    q_order[4] = (rand() % 6) + 4;
    for (int i = 5; i <= 9; i++)
    {
        repeat = 1;
        while (repeat == 1)
        {
            temp = (rand() % 6) + 4;
            for (int j = 4; j < i; j++)
            {
                if (temp == q_order[j])
                {
                    repeat = 1;
                    break;
                }
                else
                    repeat = 0;
            }
        }
        q_order[i] = temp;
    }

    //To ensure that last 8 places of q_order[18] array are filled with integers 10-17 in random fashion, without repetition.
    q_order[10] = (rand() % 8) + 10;
    for (int i = 11; i <= 17; i++)
    {
        repeat = 1;
        while (repeat == 1)
        {
            temp = (rand() % 8) + 10;
            for (int j = 10; j < i; j++)
            {
                if (temp == q_order[j])
                {
                    repeat = 1;
                    break;
                }
                else
                    repeat = 0;
            }
        }
        q_order[i] = temp;
    }
}

//This will take care of a standard thing used quite often in the game - prompting the user to press [ENTER] key to continue + ensuring that user does the same.
void enter_to_cont()
{
    int c;
    while (1)
    {
        printf("press [ENTER] key to continue\n");

        c = getchar();
        if (c == '\n') //to ensure that the user has pressed only the [ENTER] key.
            break;
    }
}

//Question Bank of the KBC game is contained within this function.
//Each segment (i.e. easy, medium and difficult) carries an extra question to take care of the lifeline2(Flip the ques.)
char *questions(int q_disp)
{
    char *ques[18];

    ques[0] = "The International Literacy Day is observed on :\n A. May 2\n B. September 8\n C. October 26\n D. June 30\n";
    ques[1] = "Which of these mammals lay eggs?\n A. Platypus\n B. Blue Whale\n C. Dolphins\n D. Hyenas\n";
    ques[2] = "Which country coined the name 'Nisarga' for the cyclon that recently hit some coastal parts of India?\n A. Pakistan\n B. Nepal\n C. Sri Lanka\n D. Bangladesh\n";
    ques[3] = "Novak Djokovic is a top ranked player of which sports?\n A. Table Tennis\n B. Badminton\n C. Lawn Tennis\n D. Squash\n";
    ques[4] = "Which of these scientists does not have a chemical element on the periodic table named after him?\n A. Albert Einstein\n B. Alfred Nobel\n C. Thomas Edison\n D. Enrico Fermi\n";
    ques[5] = "Which of the following was the theme of the World Red Cross and Red Crescent Day?\n A. Dignity for all - focus on women\n B. Dignity for all - focus on children\n C. Focus on health for all\n D. Nourishment for all-focus on children\n";
    ques[6] = "Who is the author of 'Manas Ka-Hans'?\n A. Khushwant Singh\n B. Prem Chand\n C. Jayashankar Prasad\n D. Amrit Lal Nagar\n";
    ques[7] = "The Lalit Kala Academy is devoted to the promotion of :\n A. Dance & Drama\n B. Fine Arts\n C. Literature\n D. Music\n";
    ques[8] = "Which of the telescope captured first ever Black hole image?\n A. James Webb Space Telescope\n B. Hubble Space Telescope\n C. Event Horizon Telescope\n D. Spitzer Space Telescope\n";
    ques[9] = "Which of the following years was celebrated as the 'World Communication Year'?\n A. 1983\n B. 1981\n C. 1979\n D. 1984\n";
    ques[10] = "Who was the Indian bowler off whom Australian legend Don Bradman got a single to reach his 100th first-class century?\n A. Baqa Jilani\n B. Commandur Rangachari\n C. Gogumal Kishenchand\n D. Kanwar Rai Singh\n";
    ques[11] = "Among whom does the Indian Constitution permit to take part in the Proceedings of Parliament?\n A. Solicitor General\n B. Attorney General\n C. Cabinet Secretary\n D. Chief Justice\n";
    ques[12] = "Which of the following forts was NOT built by the European colonial powers in India?\n A. Fort Dansborg\n B. Fort Naarden\n C. Fort Santa Katherina\n D. Fort Chambray\n";
    ques[13] = "Who is the first woman to successfully climb K2, the world's second highest mountain peak?\n A. Junko Tabei\n B. Wanda Rutkiewicz\n C. Tamae Watanabe\n D. Chantal Mauduit\n";
    ques[14] = "What was the name given to the supercluster of galaxies, discovered by a team of Indian astronomers in 2017?\n A. Laxmi\n B. Parvati\n C. Saraswati\n D. Durga\n";
    ques[15] = "Which insect shorted out an early supercomputer and inspired the term 'computer bug'?\n A. Moth\n B. Roach\n C. Fly\n D. Japanese beetle\n";
    ques[16] = "Which of the following landlocked countries is entirely contained within another country?\n A. Lesotho\n B. Burkina Faso\n C. Mongolia\n D. Luxembourg\n";
    ques[17] = "Who is credited with inventing the first mass-produced helicopter?\n A. Gottlieb Daimler\n B. Elmer Sperry\n C. Ferdinand von Zeppelin\n D. Igor Sikorsky\n";

    return ques[q_disp];
}

//This function accomplishes a very important task of clearing all of the buffer left in the std input stream.
//Also this will come handy for dealing with user input having more than one characters...
void clear_stdin()
{
    int buffer;
    //clear the input buffer one by one
    do {
        buffer = getchar();
    } while (buffer != '\n');
}
