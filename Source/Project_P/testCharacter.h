// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "testCharacter.generated.h"

UCLASS()
class PROJECT_P_API AtestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AtestCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float speed = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Movement Functions
	void MoveForward(float value);
	void MoveSide(float value);

	// Look Functions
	void LookUp(float value);
	void LookSide(float value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
