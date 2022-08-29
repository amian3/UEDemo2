// Fill out your copyright notice in the Description page of Project Settings.


#include "BridgeBlock.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
// Sets default values
ABridgeBlock::ABridgeBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Block"));
	RootComponent = Block;

	for (int i = 0; i < 6; ++i)
	{
		Socket.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*FString::FromInt(i))));
		Socket[i]->AttachTo(RootComponent);
	}
	for (int i = 0; i < 6; ++i)
	{
		Constraint.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(FName(*FString::FromInt(i + 6))));
		Constraint[i]->SetupAttachment(Socket[i]);
		//if (Socket[i]->GetComponentLocation())
		//{
		FVector Temp = Socket[i]->GetComponentLocation();
		Constraint[i]->SetWorldLocation(Temp);
		//}
		Constraint[i]->SetConstrainedComponents(Socket[i], Socket[i]->GetFName(), Block, Block->GetFName());
		Constraint[i]->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
		Constraint[i]->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
		Constraint[i]->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

	}
}

// Called when the game starts or when spawned
void ABridgeBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABridgeBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

