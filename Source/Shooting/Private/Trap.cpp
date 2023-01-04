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


	// USphereComponent�� ��Ʈ �ݸ��� ����
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(200);

	// UstaticMesh�� ��Ʈ�� �ڽ� ������Ʈ�� ����
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(4.0f));
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �浹 �������� "TrapPreset" ���� ����
	SphereComponent->SetCollisionProfileName(TEXT("TrapPreset"));
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel6);

}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
	// overlap ��������Ʈ�� InTrap �Լ��� ����
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrap::InTrap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATrap::OutTrap);


}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// ���� ���� �ð�(CurrentTime)�� ������ �ð�(TeleportTime)�� �����ϸ�
	if (CurrentTime >= TeleportTime)
	{
		// ������ ��ġ(����)�� ����
		float RandomY = FMath::RandRange(-200.0f, 200.0f);
		float RandomZ = FMath::RandRange(-600.0f, 600.0f);

		/*RandomLocation.Y = RandomY;
		RandomLocation.Z = RandomZ;*/
		FVector RandomLocation = FVector(0.0f, RandomY, RandomZ);
		
		/*FVector RandomLocation = FMath::VRand() * 500.0f;
		RandomLocation.X = 0.0f;*/

		// �ش� ��ġ�� �̵�
		// SetActorLocation(RandomLocation);
		TeleportTo(RandomLocation, GetActorRotation());

		// ���� �ð�(CurrentTime) �ʱ�ȭ
		CurrentTime = 0.0f;
	}
	// �׷��� �ʴٸ�
	else
	{
		// �ð��� ����
		CurrentTime += DeltaTime;
	}

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