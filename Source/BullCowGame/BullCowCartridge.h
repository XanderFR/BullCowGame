// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount //public structre available for modification
{
	int32 Bulls = 0; //accessible everywhere in program
	int32 Cows = 0; //accessible everywhere in program
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void SetupGame();
	void EndGame();
	void ProcessGuess(const FString& Guess);
	bool IsIsogram(const FString& Word) const;
	TArray<FString> GetValidWords(const TArray<FString>& WordList) const;
	FBullCowCount GetBullCows(const FString& Guess) const;

	// Your declarations go below!
	//const after a function name is a promise not to change private member variables
	private:
	FString HiddenWord; //Available everywhere, each instance of the class has unique hidden word
	int32 Lives;
	bool bGameOver;
	TArray<FString> Isograms;
};
