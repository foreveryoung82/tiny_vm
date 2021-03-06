import argparse


from interactive_mode import Console


def _interactive_mode():
    print('interactive mode processing ...')
    con = Console()
    con.process()

def _file_mode(file_url):
    print('input file: "{}"'.format(file_url))
    print('file mode processing ...')


def main():
    parser = argparse.ArgumentParser(
        description='execute algerbra expression')
    parser.add_argument('--file', '-f')
    parser.add_argument('--interactive',
        '-i',
        action='store_true',
        default=True)
    args = parser.parse_args()
    
    if args.interactive:
        _interactive_mode()
    else:
        _file_mode(args.file)

if __name__ == "__main__":
    main()