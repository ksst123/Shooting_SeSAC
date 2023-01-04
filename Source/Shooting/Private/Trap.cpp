// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShootingFlight.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// USphereComponent로 루트 콜리전 생성
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(200);

	// UstaticMesh를 루트의 자식 컴포넌트로 생성
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(4.0f));
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));

	// 충돌 프리셋을 "TrapPreset" 으로 설정
	SphereComponent->SetCollisionProfileName(TEXT("TrapPreset"));
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel6);

}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
	// overlap 델리게이트에 InTrap 함수를 연결
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrap::InTrap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATrap::OutTrap);


}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrap::InTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	AShootingFlight* player = Cast<AShootingFlight>(OtherActor);

	if (player != nullptr)
	{
		player->IsTrapped = true;
	}
}

void ATrap::OutTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	AShootingFlight* player = Cast<AShootingFlight>(OtherActor);
	if (player != nullptr)
	{
		player->IsTrapped = false;
	}
}