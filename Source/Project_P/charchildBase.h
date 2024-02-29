// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "NavigationSystem.h" 
#include "charchildBase.generated.h"

UCLASS()
class PROJECT_P_API AcharchildBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AcharchildBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector originPosition;
	AAIController* aiController;


	void MoveIA();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
