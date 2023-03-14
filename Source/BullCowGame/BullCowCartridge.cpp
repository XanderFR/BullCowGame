// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words); //list of isograms

    SetupGame();//Setting up the game
    //Wrap Unreal strings within a TEXT() Macro 
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter key
{
    if (bGameOver)
    { //if the game is over
        ClearScreen();
        SetupGame();
    }
    else //if the game is not over, check player guess
    {
        ProcessGuess(Input);
    }
    
}

void UBullCowCartridge::SetupGame() 
{

    //Welcoming the player
    PrintLine(TEXT("Welcome to the Bull Cow Game!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; //Sets the HiddenWord
    Lives = HiddenWord.Len();//Set some lives based on hidden word length
    bGameOver = false; //the game is not over

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Type in your guess and \nPress enter to continue..."));//prompt player for guess 
    PrintLine(TEXT("The Hidden Word is %s."), *HiddenWord); //Debug line

}

void UBullCowCartridge::EndGame() 
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord) //If the player guesses correctly
    {
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    }
    
    //Prompt to guess again
    //Check right number of characters
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i characters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining"), Lives);
        return;
    }

    //Check if isogram
    if (!IsIsogram(Guess))
    {
        /* code */
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    //Remove life
    PrintLine(TEXT("Lost a life!"));
    --Lives;

    if (Lives <= 0) //If player out of lives
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!")); //tell player ran out of lives
        PrintLine(TEXT("The Hidden Word was: %s"), *HiddenWord); //reveal hidden word to player through its pointer
        EndGame(); //end the game
        return;
    }

    //Show the player the bulls and the cows
    FBullCowCount Score = GetBullCows(Guess); //prepares an FBullCowCount struct named score whose values are the result of the GetBullCows function
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again, you have %i lives left"), Lives); //tell player the lives remaining

}

bool UBullCowCartridge::IsIsogram(const FString& Word) const //functions that do no alter private member variables should have const keyword
{
    //nested for loop that compares every letter in Word to its subsequent letters, trying to find matches
    for (int32 Index = 0; Index < Word.Len(); Index++) //stops berfore the null character
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++) //stops berfore the null character
        {//comparison of subsequent characters
            if (Word[Index] == Word[Comparison]) //if there are any similar characters
            {
                return false; //not isogram
            }
        }
    }

    return true; //is an isogram

}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords; //create an array to hold word of a certain length

    for (FString Word : WordList) //a for each loop
    {
        if (4 <= Word.Len() && Word.Len() <= 8 && IsIsogram(Word)) //if a word meet length and isogram requirements
        {
            ValidWords.Emplace(Word); //adds words to the end of ValidWords
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count; //prepare an FBullCowCount struct named Count with Bulls and Cows both starting at 0;

    //for every index guess is same as index hidden, BullCount++
    //if not a bull was it a cow? if yes CowCount++

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++) //for loop that travels along the player guess word
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex]) //see if the current letter in guess matches the letter in hidden word in the same spot
        {
            Count.Bulls++; //Bulls are correct letters in the right place, increment the Bulls value of Count
            continue;
        }
        //nested for loop that compares every letter in hidden word to the current value of Guess[GuessIndex]
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++) //loop that travels along the hidden word
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex]) //if the current letter of player guess is the same as the current value of HiddenWord[HiddenIndex]
            {
                Count.Cows++; //Cows are correct letter in the wrong place, increment the Cows value of Count
                break;
            }
        }
        
    }
    return Count; //return the FBullCowCount struct
}