new_http_archive(
	name = "gmock_archive",
	url = "https://googlemock.googlecode.com/files/gmock-1.7.0.zip",
	sha256 = "26fcbb5925b74ad5fc8c26b0495dfc96353f4d553492eb97e85a8a6d2f43095b",
	build_file = "bazel/gmock.BUILD",
)

bind(
	name = "gmock",
	actual = "@gmock_archive//:gmock",
)

git_repository(
	name = "protobuf_repo",
	remote = "https://github.com/google/protobuf.git",
	commit = "48ebb29a8ec118bf6b9ee39f6be42b57321c099a",
	init_submodules = 1,
)

bind(
	name = "protobuf",
	actual = "@protobuf_repo//:protobuf",
)

bind(
	name = "protoc",
	actual = "@protobuf_repo//:protoc",
)

new_http_archive(
	name = "zlib_archive",
	url = "http://zlib.net/zlib128.zip",
	sha256 = "879d73d8cd4d155f31c1f04838ecd567d34bebda780156f0e82a20721b3973d5",
	build_file = "bazel/zlib.BUILD",
)

bind(
	name = "zlib",
	actual = "@zlib_archive//:zlib",
)

new_git_repository(
	name = "jemalloc_repo",
	remote = "https://github.com/jemalloc/jemalloc.git",
	tag = "4.1.0",
	init_submodules = 1,
	build_file = "bazel/jemalloc.BUILD",
)

bind(
	name = "jemalloc",
	actual = "@jemalloc_repo//:jemalloc",
)
