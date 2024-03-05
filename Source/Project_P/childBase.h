// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "TimerManager.h" 
#include "childBase.generated.h"

UCLASS()
class PROJECT_P_API AchildBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AchildBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Simplifier Speed character
	/** Multiply the walk speed (base : 600) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
		float walkSpeed = 600;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
		//TArray<FNavLocation> navLocationArray;

	FVector originPosition;
	FNavLocation goalLocation;
	AAIController* aiController;
	UNavigationSystemV1* navSystem;
	FTimerHandle TimerHandle;
	FPathFollowingRequestResult moveResult;




	int count = 0.0f;

	void OnMoveCompleted(EPathFollowingRequestResult::Type Result);
	virtual FPathFollowingRequestResult MoveIA(FNavLocation goalLocation);
	
	
	void moveChild();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
