#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../include/AssetHandler.hpp"
#include "../include/AssetTypes.hpp"

using namespace Engine::AssetHandling;

namespace Engine::AssetHandling
{
    struct TestAsset : Asset
    {
        int value = 0;
    };

    template <>
    struct AssetTraits<TestAsset>
    {
        using Handle = Assets::AssetId<struct TestTag>;

        static TestAsset Load(AssetLoadContext, const std::string& name)
        {
            TestAsset a;
            a.value = static_cast<int>(name.size());
            return a;
        }
    };
}

struct TestAssetCacheGuard
{
    ~TestAssetCacheGuard()
    {
        AssetHandler::ClearCache<TestAsset>();
    }
};

TEST_CASE("AssetHandler - Register and GetAsset", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;

    TestAsset testAsset;
    testAsset.value = 42;

    auto handle = handler.RegisterAsset<TestAsset>(std::move(testAsset));

    REQUIRE(handle);

    auto loaded = handler.GetAsset<TestAsset>(handle);
    REQUIRE(loaded->value == 42);
}

TEST_CASE("AssetHandler - Load caches assets", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;

    auto h1 = handler.LoadAsset<TestAsset>("foo");
    auto h2 = handler.LoadAsset<TestAsset>("foo");

    REQUIRE(h1 == h2);
}

TEST_CASE("AssetHandler - LoadAsset with empty name returns invalid handle", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;

    auto handle = handler.LoadAsset<TestAsset>("");
    REQUIRE_FALSE(handle);
}

TEST_CASE("AssetHandler - New asset starts with revision one", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;
    auto handle = handler.LoadAsset<TestAsset>("foo");

    auto revision = handler.GetAssetRevision<TestAsset>(handle);
    REQUIRE(revision == 1);
}

TEST_CASE("AssetHandler - Update increases revision", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;
    auto handle = handler.LoadAsset<TestAsset>("abc");

    auto r1 = handler.GetAssetRevision<TestAsset>(handle);

    handler.UpdateAsset<TestAsset>(handle, [](TestAsset& a)
    {
        a.value = 99;
    });

    auto r2 = handler.GetAssetRevision<TestAsset>(handle);
    REQUIRE(r2 == r1 + 1);

    auto asset = handler.GetAsset<TestAsset>(handle);
    REQUIRE(asset->value == 99);

    handler.UpdateAsset<TestAsset>(handle, [](TestAsset& a)
    {
        a.value = 999;
    });
    handler.UpdateAsset<TestAsset>(handle, [](TestAsset& a)
    {
        a.value = 14;
    });

    auto r3 = handler.GetAssetRevision<TestAsset>(handle);
    REQUIRE(r3 == r1 + 3);

    asset = handler.GetAsset<TestAsset>(handle);
    REQUIRE(asset->value == 14);
}

TEST_CASE("Asset Handler - Find returns existing handle", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;
    auto h1 = handler.LoadAsset<TestAsset>("foo");
    auto found = handler.FindAsset<TestAsset>("foo");
    REQUIRE(found.has_value());
    REQUIRE(found.value() == h1);

    auto found_unregistered = handler.FindAsset<TestAsset>("bar");
    REQUIRE_FALSE(found_unregistered.has_value());
}

TEST_CASE("Asset Handler - Invalid handle throws", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;

    AssetId<struct TestTag> invalid{0};
    REQUIRE_THROWS(handler.GetAsset<TestAsset>(invalid));
}

TEST_CASE("Asset Handler - GetAllAssetHandlesOFType", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;

    auto h1 = handler.LoadAsset<TestAsset>("foo");
    auto h2 = handler.LoadAsset<TestAsset>("bar");

    auto all = handler.GetAllAssetHandlesOfType<TestAsset>();

    REQUIRE(all.size() == 2);
}

TEST_CASE("Asset Handler - Get Asset handle from name", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;
    auto h1 = handler.LoadAsset<TestAsset>("foo");

    auto received_handle = handler.GetHandleFromName<TestAsset>("foo");
    REQUIRE(received_handle == h1);

    REQUIRE_THROWS(handler.GetHandleFromName<TestAsset>("bar"));
}

TEST_CASE("Asset Handler - Load Asset without caching", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;

    auto asset = handler.LoadAssetWithoutCaching<TestAsset>("foo");

    REQUIRE(asset.value == 3);

    REQUIRE_THROWS(handler.GetHandleFromName<TestAsset>("foo"));
}

TEST_CASE("Asset Handler - Get Asset Revision with invalid handle", "[AssetHandling]")
{
    TestAssetCacheGuard guard;
    AssetHandler handler;

    AssetId<struct TestTag> invalid{0};
    REQUIRE_THROWS(handler.GetAssetRevision<TestAsset>(invalid));
}
