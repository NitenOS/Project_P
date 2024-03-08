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

	count += DeltaTime;
	
	if (!navPoints.IsEmpty()) {
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Purple, FString(FString::SanitizeFloat(navPoints.Num())));
		if (navPoints[navPointCount] != FVector(0, 0, 0)) {
			goalLocation = FNavLocation(navPoints[navPointCount]);
		}
	}

	//if (count >= waitTime) {
	//	moveChild();
	//	count = 0;
	//}
}

void AchildBase::moveChild() {

	
	moveResult = MoveIA(goalLocation);
	OnMoveCompleted(moveResult);
}

FPathFollowingRequestResult AchildBase::MoveIA(FNavLocation _goalLocation) {
	FAIMoveRequest moveRequest = FAIMoveRequest();
	moveRequest.SetAcceptanceRadius(acceptedRadius);

	moveRequest.SetGoalLocation(_goalLocation.Location);
	return aiController->MoveTo(moveRequest);
}

void AchildBase::OnMoveCompleted(EPathFollowingRequestResult::Type Result)
{
	switch (Result) {
	case EPathFollowingRequestResult::Failed:
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Purple, FString(TEXT("Request failed")));
		break;
	case EPathFollowingRequestResult::AlreadyAtGoal:
		//GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Purple, FString(TEXT("Already at Goal !")));

		//navSystem->GetRandomReachablePointInRadius(originPosition, 500.0f, goalLocation);
		if (navPointCount == navPoints.Num()-1) navPointCount = -1;
		navPointCount++;
		oldNavPoint = navPointCount;

		do { navPointCount = FMath::RandRange(0, navPoints.Num() - 1);

		} while (navPointCount == oldNavPoint);

		moveResult = MoveIA(goalLocation);
		break;

	case EPathFollowingRequestResult::RequestSuccessful:
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Purple, FString(TEXT("Request Succes !")));
		break;
	}
}


FNavLocation AchildBase::getGoalLocation() {
	return goalLocation;
}

void AchildBase::setGoalLocation(FNavLocation _goalLocation) {
	goalLocation = _goalLocation;
}