// Fill out your copyright notice in the Description page of Project Settings.


#include "childBase.h"

// Sets default values
AchildBase::AchildBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AchildBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	// Setup and move child first
	{
		originPosition = GetActorLocation();
		aiController = this->GetController<AAIController>();

		goalLocation = FNavLocation(originPosition);
		navSystem = UNavigationSystemV1::GetCurrent(GetWorld());

		navSystem->GetRandomReachablePointInRadius(originPosition, 1.0f, goalLocation);
		moveResult = MoveIA(goalLocation);
	}


}

// Called every frame
void AchildBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	count += 1;
	
	if (count == 100) {
		moveChild();
		count = 0;
	}
}

void AchildBase::moveChild() {

	
	moveResult = MoveIA(goalLocation);
	OnMoveCompleted(moveResult);
}

// Called to bind functionality to input
void AchildBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveChild", IE_Pressed, this, &AchildBase::moveChild);
}

FPathFollowingRequestResult AchildBase::MoveIA(FNavLocation _goalLocation) {
	FAIMoveRequest moveRequest = FAIMoveRequest();

	moveRequest.SetGoalLocation(_goalLocation.Location);
	return aiController->MoveTo(moveRequest);
}

void AchildBase::OnMoveCompleted(EPathFollowingRequestResult::Type Result)
{
	switch (Result) {
	case EPathFollowingRequestResult::Failed:
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Purple, FString(TEXT("Request failed")));
		break;
	case EPathFollowingRequestResult::AlreadyAtGoal:
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Purple, FString(TEXT("Already at Goal !")));

		navSystem->GetRandomReachablePointInRadius(originPosition, 500.0f, goalLocation);
		moveResult = MoveIA(goalLocation);
		break;
	case EPathFollowingRequestResult::RequestSuccessful:
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Purple, FString(TEXT("Request Succes !")));
		break;
	}
}