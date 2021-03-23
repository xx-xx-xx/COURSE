#include <stdio.h>
#include <string.h>

char reflector[] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
char rotor_table[6][27] =
    {
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
        "AJDKSIRUXBLHWTMCQGZNPYFVOE",
        "BDFHJLCPRTXVZNYEIWGAKMUSQO",
        "ESOVPZJAYQUIRHXLNFTGKDCMWB",
        "VZBRGITYUPSDNHLXAWMJQOFECK"};
char step_char[7] = " RFWKA";

char plugboard[30] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char ciphertext[501], plaintext[501], word[21];
char ringsetting[3], messagekey[3];
int rotornum[4];
int length_text, length_word;

void rotornum_traverse();
void shift();
int check_word();
char enigma_decrypt(char text);

int main()
{
    int i, j;
    char temp, plug[2][11];

    for (i = 0; i < 10; i++)
    {
        scanf("%c%c", &plug[0][i], &plug[1][i]);
        if (i != 9)
            scanf(" ");
        temp = plugboard[plug[0][i] - 'A'];
        plugboard[plug[0][i] - 'A'] = plugboard[plug[1][i] - 'A'];
        plugboard[plug[1][i] - 'A'] = temp;
    }

    scanf("%s%s%s%d", ringsetting, ciphertext, word, &rotornum[0]);

    length_text = strlen(ciphertext);
    length_word = strlen(word);

    rotornum_traverse();

    printf("MessageKey=%c%c%c\n", messagekey[0] + 'A', messagekey[1] + 'A', messagekey[2] + 'A');
    printf("PlainText=%s\n", plaintext);
    printf("RotorNum=%d%d%d", rotornum[0], rotornum[1], rotornum[2]);
    return 0;
}

void rotornum_traverse()
{
    int i, j, k, m, n, s;
    for (i = 1; i <= 5; i++)
    {
        for (j = 1; j <= 5; j++)
        {
            if (i == rotornum[0] || j == i || j == rotornum[0])
                continue;
            rotornum[1] = i;
            rotornum[2] = j;

            for (k = 0; k < 26; k++)
            {
                for (m = 0; m < 26; m++)
                {
                    for (n = 0; n < 26; n++)
                    {
                        messagekey[0] = k;
                        messagekey[1] = m;
                        messagekey[2] = n;

                        for (s = 0; s < length_text; s++)
                            plaintext[s] = enigma_decrypt(plugboard[ciphertext[s] - 'A'] - 'A');
                        plaintext[length_text] = '\0';

                        if (check_word() == 1)
                        {
                            messagekey[0] = k;
                            messagekey[1] = m;
                            messagekey[2] = n;
                            return;
                        }
                    }
                }
            }
        }
    }
}
char enigma_decrypt(char text)
{
    int i, j;
    int tmp[3];

    shift();

    for (i = 0; i <= 2; i++)
        tmp[i] = (messagekey[i] - ringsetting[i] + 'A' + 26) % 26;

    for (i = 2; i >= 0; i--)
    {
        text = (text + tmp[i]) % 26;
        text = rotor_table[rotornum[i]][text] - 'A';
        text = (text - tmp[i] + 26) % 26;
    }

    text = reflector[text] - 'A';

    for (j = 0; j < 3; j++)
    {
        text = (text + tmp[j]) % 26;
        for (i = 0; i < 26; i++)
            if (text == (rotor_table[rotornum[j]][i] - 'A'))
                break;
        text = i;
        text = (text - tmp[j] + 26) % 26;
    }

    return plugboard[text];
}
void shift()
{
    if (messagekey[1] == ((step_char[rotornum[1]] - 'A' + 25) % 26))
        messagekey[0] = (messagekey[0] + 1) % 26;
    if (messagekey[2] == ((step_char[rotornum[2]] - 'A' + 25) % 26) || messagekey[1] == ((step_char[rotornum[1]] - 'A' + 25) % 26))
        messagekey[1] = (messagekey[1] + 1) % 26;
    messagekey[2] = (messagekey[2] + 1) % 26;
}
int check_word()
{
    int i, j;
    int flag;
    for (i = 0; i <= length_text - length_word; i++)
    {
        flag = 1;
        for (j = 0; j < length_word; j++)
            if (plaintext[i + j] != word[j])
                flag = 0;

        if (flag == 1)
            break;
    }

    return flag;
}